/* The queue of GS work and the worker threads that do it.

   gs_draw_prim() snapshots the draw state (one GsDraw per change of the
   registers, kept in a ring) and appends the primitive to a ring of
   commands. Every worker reads every command, in order, and renders the
   rows it owns (GsBand: chunks of 1 << GRX_GS_BAND rows dealt out in
   turn), so a pixel is always drawn by the same thread, in the order the
   primitives came, and the result is the same as drawing everything on one
   thread.

   That holds as long as what the queued commands touch is addressed the
   same way by all of them. The queue tracks pages (8 KB) of local memory:
   since the last barrier, the pages draws read as textures, the pages
   they draw into (with how: buffer, width, format and position, the
   "key") and the pages queued uploads write; and puts a barrier (every
   worker finishes everything before it first) before
   - a draw that reads a texture from pages queued draws or uploads write;
   - a draw that writes pages queued draws read as textures or uploads
     write, or that queued draws write through another key (another
     buffer, width or format over the same memory);
   - an upload into pages queued draws or uploads use.
   An image upload (host to local) that would write pages the queue still
   uses goes into the queue too, with a copy of its data; the workers write
   it, each its own rows. Otherwise it is written at once, on the calling
   thread. Anything else that reads local memory outside the draws (CLUT
   loads, the display) waits for the queued commands that write what it
   reads (for each page, the queue keeps the last command that writes it
   and the last that reads it); a CLUT load whose data is in a queued
   upload takes it from the upload's data instead (gs_queue_writer).
   Read-back and local-to-local copies wait for everything (gs_flush).

   A sprite that reads as a texture the pages it draws into (copying the
   frame onto itself) is split around a barrier when its rows allow
   (sprite_split); any other draw that does, or whose frame and Z overlap,
   or whose buffer wraps onto itself, is drawn by one worker alone between
   two barriers, as the GS would.

   At each vsync the work queued before the previous vsync must be done
   (gs_queue_vsync): the GS stays within a field of the game.

   GRX_GS_THREADS=n sets the number of workers (0: everything on the
   calling thread); the default is half the logical CPU count (about one
   per core), at most 16. GRX_GS_BAND=s sets the band height to 1 << s rows (2 to 8, default
   3). GRX_GS_SPIN_US=n: how long an idle worker spins before it sleeps
   (default 50). GRX_GS_STATS=1 prints what the queue did at exit; 2 also
   logs each wait at a vsync. */
#include <math.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "gs_draw.h"

#define QCAP 16384 /* commands; a power of two */
#define SCAP 512   /* draw states */
#define ARENA (16u << 20) /* upload data, a ring */
#define ACHUNK 16           /* in chunks of 1 << ACHUNK bytes */
#define MAXW 32
static uint64_t worker_spin_ns = 50000; /* how long an idle worker spins before it sleeps */
#define MAIN_SPIN_NS 200000ull

/* A primitive (type 0-7), drawn on rows y0..y1-1 only (at least all the
   rows it touches); the same drawn by worker 0 alone; a barrier; an
   upload */
enum { CMD_SOLO = 8, CMD_BARRIER = 16, CMD_UPLOAD = 17 };
typedef struct GsCmd {
    const GsDraw *d;
    const GsUpload *up;
    uint32_t type;
    int32_t y0, y1;
    GsVertex v[3];
} GsCmd;

static GsCmd *ring;
static GsDraw *states;
static uint32_t state_last[SCAP]; /* the seq after the last command that used the state */
static uint32_t cur_slot;
static int cur_used; /* commands use the current state */
static GsDraw *cur;
static GsDraw solo_state; /* without workers */
static uint8_t *arena;
static uint32_t arena_pos;
static int arena_open = -1; /* the chunk being filled */
/* The last uploads queued, by seq */
#define UPHIST 32
static struct {
    const GsUpload *u;
    uint32_t seq;
} uphist[UPHIST];
static uint32_t uphist_n;
static uint32_t arena_last[ARENA >> ACHUNK]; /* the seq after the last command that uses each chunk */

static uint32_t seq; /* commands queued so far */
static atomic_uint pub;
static struct {
    atomic_uint done;
    char pad[60];
} wdone[MAXW];
static atomic_int sleepers, main_waiting;
static atomic_uint main_target; /* what the calling thread waits for */
static int nworkers = -1, band_shift = 3;
/* Idle workers wait for work, workers at a barrier for each other, the
   calling thread for the workers: each with its own lock */
static SDL_Mutex *mtx_work, *mtx_bar, *mtx_done;
static SDL_Condition *cv_work, *cv_done, *cv_bar;

/* Since the last barrier: pages queued draws write (with their keys) and
   read, pages queued uploads write */
static GsPages aw, ar, uw;
static uint32_t wkey[512];
/* For each page, the seq after the last command that writes it, and that
   reads it (0: none since the last flush) */
static uint32_t page_w[512], page_r[512];
static int pending; /* commands queued since the last flush */

static const GsBand band_all = {0, 1, 0, 0, 0x7FFFFFFF};

/* GRX_GS_STATS=1: what the queue did, at exit */
enum { F_CALL, F_READ, F_DISPLAY, F_ROOM, F_VSYNC, F_N };
static const char *const flush_names[F_N] = {"explicit", "memory read", "display", "arena", "vsync"};
enum { B_TEX, B_WRITE, B_UPLOAD, B_SOLO, B_SPLIT, B_N };
static const char *const barrier_names[B_N] = {"texture read", "draw over", "upload", "drawn alone", "split"};
static uint64_t st_flush[F_N], st_wait_why[F_N], st_barrier[B_N], st_cmds, st_states, st_full_ns, st_uploads,
    st_upload_bytes, st_forward, st_busy_ns[MAXW], st_bar_ns[MAXW];
static int stats;
static void print_stats(void);
static uint32_t min_done(void);

/* ---- page sets ---- */

static inline void pages_add(GsPages *s, uint32_t p) {
    s->w[(p >> 5) & 15] |= 1u << (p & 31);
}
static inline int pages_has(const GsPages *s, uint32_t p) {
    return (s->w[(p >> 5) & 15] >> (p & 31)) & 1;
}
static inline int pages_meet(const GsPages *a, const GsPages *b) {
    uint32_t m = 0;
    int i;
    for (i = 0; i < 16; i++)
        m |= a->w[i] & b->w[i];
    return m != 0;
}

void gs_pages_rect(GsPages *s, uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t w,
                   uint32_t h) {
    uint32_t pw_, ph, bwp, c0, c1, r0, r1, r, c;
    int extra = (bp & 31) != 0;

    if (w == 0 || h == 0)
        return;
    switch (gs_psm[psm & 63].layout) {
    case GS_L32: case GS_L32Z: pw_ = 64; ph = 32; bwp = bw; break;
    case GS_L16: case GS_L16S: case GS_L16Z: case GS_L16SZ: pw_ = 64; ph = 64; bwp = bw; break;
    case GS_L8: pw_ = 128; ph = 64; bwp = gs_bw8(bw); break;
    default: pw_ = 128; ph = 128; bwp = gs_bw8(bw); break;
    }
    if (x + w > 2048 || y + h > 2048) {
        memset(s, 0xFF, sizeof(*s));
        return;
    }
    c0 = x / pw_;
    c1 = (x + w - 1) / pw_;
    r0 = y / ph;
    r1 = (y + h - 1) / ph;
    if ((r1 - r0 + 1) * (c1 - c0 + 1) > 512) {
        memset(s, 0xFF, sizeof(*s));
        return;
    }
    for (r = r0; r <= r1; r++)
        for (c = c0; c <= c1; c++) {
            uint32_t p = (bp >> 5) + r * bwp + c;
            pages_add(s, p & 511);
            if (extra)
                pages_add(s, (p + 1) & 511);
        }
}

/* ---- workers ---- */

static inline int band_touches(const GsBand *b, int y0, int y1) {
    uint32_t c0, c1, c;
    if (y0 >= y1)
        return 0;
    c0 = (uint32_t)y0 >> b->sh;
    c1 = (uint32_t)(y1 - 1) >> b->sh;
    if (c1 - c0 + 1 >= (uint32_t)b->n)
        return 1;
    for (c = c0; c <= c1; c++)
        if ((int)(c % (uint32_t)b->n) == b->k)
            return 1;
    return 0;
}

/* Every worker has finished the commands before seq number `target` */
static int reached(uint32_t target) {
    int k;
    for (k = 0; k < nworkers; k++)
        if ((int32_t)(atomic_load(&wdone[k].done) - target) < 0)
            return 0;
    return 1;
}

/* After a worker's done moved on: wake the calling thread if what it
   waits for is done now */
static void wake_main(void) {
    if (atomic_load(&main_waiting) && reached(atomic_load(&main_target))) {
        SDL_LockMutex(mtx_done);
        SDL_BroadcastCondition(cv_done);
        SDL_UnlockMutex(mtx_done);
    }
}

/* Wait at a barrier (command s) until every worker has done everything
   before it */
static void barrier(int k, uint32_t s) {
    uint64_t t0;
    int spins = 0;

    atomic_store(&wdone[k].done, s);
    wake_main();
    if (reached(s)) {
        SDL_LockMutex(mtx_bar);
        SDL_BroadcastCondition(cv_bar);
        SDL_UnlockMutex(mtx_bar);
        return;
    }
    t0 = SDL_GetTicksNS();
    while (!reached(s)) {
        SDL_CPUPauseInstruction();
        if (++spins == 64) {
            spins = 0;
            if (SDL_GetTicksNS() - t0 > worker_spin_ns)
                break;
        }
    }
    SDL_LockMutex(mtx_bar);
    while (!reached(s))
        SDL_WaitConditionTimeout(cv_bar, mtx_bar, 1);
    SDL_UnlockMutex(mtx_bar);
}

static int SDLCALL worker(void *arg) {
    int k = (int)(intptr_t)arg;
    GsBand band, rows;
    uint32_t pos = 0;

    band.k = k;
    band.n = nworkers;
    band.sh = band_shift;
    band.ylo = 0;
    band.yhi = 0x7FFFFFFF;
    rows = band;
    for (;;) {
        uint32_t avail = atomic_load(&pub);
        uint64_t t0;
        if (avail == pos) {
            int spins = 0;
            t0 = SDL_GetTicksNS();
            while (atomic_load(&pub) == pos) {
                SDL_CPUPauseInstruction();
                if (++spins == 256) {
                    spins = 0;
                    if (SDL_GetTicksNS() - t0 > worker_spin_ns)
                        break;
                }
            }
            if (atomic_load(&pub) == pos) {
                SDL_LockMutex(mtx_work);
                atomic_fetch_add(&sleepers, 1);
                while (atomic_load(&pub) == pos)
                    SDL_WaitCondition(cv_work, mtx_work);
                atomic_fetch_sub(&sleepers, 1);
                SDL_UnlockMutex(mtx_work);
            }
            continue;
        }
        t0 = stats ? SDL_GetTicksNS() : 0;
        while (pos != avail) {
            const GsCmd *c = &ring[pos & (QCAP - 1)];
            switch (c->type) {
            case CMD_BARRIER:
                if (stats) {
                    uint64_t b0 = SDL_GetTicksNS();
                    barrier(k, pos);
                    st_bar_ns[k] += SDL_GetTicksNS() - b0;
                } else {
                    barrier(k, pos);
                }
                break;
            case CMD_UPLOAD:
                gs_upload_rows(c->up, &rows);
                break;
            default:
                if (c->type & CMD_SOLO) {
                    if (k == 0) {
                        GsBand one = band_all;
                        one.ylo = c->y0;
                        one.yhi = c->y1;
                        gs_render(c->d, &one, c->v, c->type & 7);
                    }
                } else if (band_touches(&band, c->y0, c->y1)) {
                    band.ylo = c->y0;
                    band.yhi = c->y1;
                    gs_render(c->d, &band, c->v, c->type);
                }
                break;
            }
            pos++;
            if ((pos & 63) == 0)
                atomic_store(&wdone[k].done, pos);
        }
        if (stats)
            st_busy_ns[k] += SDL_GetTicksNS() - t0;
        atomic_store(&wdone[k].done, pos);
        wake_main();
    }
    return 0;
}

static void publish(void) {
    atomic_store(&pub, seq);
    if (atomic_load(&sleepers) > 0) {
        SDL_LockMutex(mtx_work);
        SDL_BroadcastCondition(cv_work);
        SDL_UnlockMutex(mtx_work);
    }
}

static void wait_for(uint32_t target) {
    uint64_t t0;
    int spins = 0;

    if (reached(target))
        return;
    t0 = SDL_GetTicksNS();
    while (!reached(target)) {
        SDL_CPUPauseInstruction();
        if (++spins == 64) {
            spins = 0;
            if (SDL_GetTicksNS() - t0 > MAIN_SPIN_NS)
                break;
        }
    }
    if (reached(target))
        return;
    atomic_store(&main_target, target);
    SDL_LockMutex(mtx_done);
    atomic_store(&main_waiting, 1);
    while (!reached(target))
        SDL_WaitConditionTimeout(cv_done, mtx_done, 1);
    atomic_store(&main_waiting, 0);
    SDL_UnlockMutex(mtx_done);
}

void gs_queue_init(void) {
    const char *e;
    int n, k;

    if (nworkers >= 0)
        return;
    nworkers = 0;
    cur = &solo_state;
    e = getenv("GRX_GS_STATS");
    if (e != NULL && atoi(e) > 0) {
        stats = atoi(e);
        atexit(print_stats);
    }
    /* At least 4 rows: in PSMT4 rows y and y + 2 (y % 4 < 2) share bytes,
       which uploads write a nibble at a time */
    e = getenv("GRX_GS_BAND");
    if (e != NULL && atoi(e) >= 2 && atoi(e) <= 8)
        band_shift = atoi(e);
    e = getenv("GRX_GS_SPIN_US");
    if (e != NULL && atoi(e) >= 0)
        worker_spin_ns = (uint64_t)atoi(e) * 1000;
    e = getenv("GRX_GS_THREADS");
    if (e != NULL && *e) {
        n = atoi(e);
    } else {
        /* About a worker per core: past that (SMT siblings) the menus
           and the attract mode run no faster, on far more CPU time */
        int cpus = SDL_GetNumLogicalCPUCores();
        n = cpus >= 4 ? cpus / 2 : cpus - 1;
        if (n > 16)
            n = 16;
    }
    if (n > MAXW)
        n = MAXW;
    if (n <= 0)
        return;
    ring = calloc(QCAP, sizeof(GsCmd));
    states = calloc(SCAP, sizeof(GsDraw));
    arena = malloc(ARENA);
    mtx_work = SDL_CreateMutex();
    mtx_bar = SDL_CreateMutex();
    mtx_done = SDL_CreateMutex();
    cv_work = SDL_CreateCondition();
    cv_done = SDL_CreateCondition();
    cv_bar = SDL_CreateCondition();
    if (!ring || !states || !arena || !mtx_work || !mtx_bar || !mtx_done || !cv_work || !cv_done || !cv_bar) {
        fprintf(stderr, "gs: no worker threads (%s)\n", SDL_GetError());
        return;
    }
    nworkers = n;
    cur = &states[0];
    for (k = 0; k < n; k++) {
        char name[16];
        SDL_Thread *t;
        snprintf(name, sizeof(name), "gs%d", k);
        t = SDL_CreateThread(worker, name, (void *)(intptr_t)k);
        if (t == NULL) {
            fprintf(stderr, "gs: SDL_CreateThread: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_DetachThread(t);
    }
}

static void flush_why(int why) {
    uint64_t t0 = 0;
    if (nworkers <= 0 || !pending)
        return;
    if (stats) {
        st_flush[why]++;
        t0 = SDL_GetTicksNS();
    }
    publish();
    wait_for(seq);
    memset(&aw, 0, sizeof(aw));
    memset(&ar, 0, sizeof(ar));
    memset(&uw, 0, sizeof(uw));
    memset(page_w, 0, sizeof(page_w));
    memset(page_r, 0, sizeof(page_r));
    memset(arena_last, 0, sizeof(arena_last));
    memset(uphist, 0, sizeof(uphist));
    memset(state_last, 0, sizeof(state_last));
    pending = 0;
    if (stats)
        st_wait_why[why] += SDL_GetTicksNS() - t0;
}

void gs_flush(void) {
    flush_why(F_CALL);
}

/* At each vsync: the work queued before the previous vsync has to be
   done, so the GS keeps up with the game to within a field, as the real
   one does with its own frame. (A vsync with no new work since the last,
   the frame loop catching up on fields it was late for, waits for
   nothing: the work of the field before must have its field.) */
void gs_queue_vsync(void) {
    static uint32_t prev;
    static int have;
    if (nworkers <= 0 || (have && seq == prev))
        return;
    if (have && pending && !reached(prev)) {
        uint64_t t0 = SDL_GetTicksNS();
        uint32_t behind = prev - min_done();
        if (stats)
            st_flush[F_VSYNC]++;
        publish();
        wait_for(prev);
        if (stats)
            st_wait_why[F_VSYNC] += SDL_GetTicksNS() - t0;
        if (stats > 1)
            fprintf(stderr, "gs: vsync waited %.2f ms for %u commands (%u queued since)%c", (SDL_GetTicksNS() - t0) / 1e6,
                    behind, seq - prev, 10);
    }
    prev = seq;
    have = 1;
}

/* The commands every worker has done: the lowest done, as a seq */
static uint32_t min_done(void) {
    uint32_t m = atomic_load(&wdone[0].done);
    int k;
    for (k = 1; k < nworkers; k++) {
        uint32_t d = atomic_load(&wdone[k].done);
        if ((int32_t)(d - m) < 0)
            m = d;
    }
    return m;
}

/* The latest of the pages' last commands (`also_r`: reads too) that isn't
   done yet, or 0 */
static uint32_t pages_last(const GsPages *p, int also_r) {
    uint32_t md, last = 0;
    int i;
    if (!pending)
        return 0;
    md = min_done();
    for (i = 0; i < 16; i++) {
        uint32_t m = p->w[i];
        while (m) {
            int pg = i * 32 + __builtin_ctz(m);
            m &= m - 1;
            if (page_w[pg] && (int32_t)(page_w[pg] - md) > 0 && (!last || (int32_t)(page_w[pg] - last) > 0))
                last = page_w[pg];
            if (also_r && page_r[pg] && (int32_t)(page_r[pg] - md) > 0 && (!last || (int32_t)(page_r[pg] - last) > 0))
                last = page_r[pg];
        }
    }
    return last;
}

static void reading(const GsPages *p, int why) {
    uint32_t last = pages_last(p, 0);
    if (last) {
        uint64_t t0 = stats ? SDL_GetTicksNS() : 0;
        if (stats)
            st_flush[why]++;
        publish();
        wait_for(last);
        if (stats)
            st_wait_why[why] += SDL_GetTicksNS() - t0;
    }
}

void gs_mem_reading(const GsPages *p) {
    reading(p, F_READ);
}

void gs_display_reading(const GsPages *p) {
    reading(p, F_DISPLAY);
}

int gs_mem_busy(const GsPages *p) {
    return pages_last(p, 1) != 0;
}

const GsUpload *gs_queue_writer(const GsPages *p) {
    uint32_t md, s = 0;
    int i;
    if (!pending)
        return NULL;
    md = min_done();
    for (i = 0; i < 16; i++) {
        uint32_t m = p->w[i];
        while (m) {
            int pg = i * 32 + __builtin_ctz(m);
            m &= m - 1;
            if (!page_w[pg] || (int32_t)(page_w[pg] - md) <= 0 || (s && page_w[pg] != s))
                return NULL;
            s = page_w[pg];
        }
    }
    for (i = 0; s && i < UPHIST; i++)
        if (uphist[i].u != NULL && uphist[i].seq == s) {
            st_forward++;
            return uphist[i].u;
        }
    return NULL;
}

static void pages_mark(uint32_t *last, const GsPages *p) {
    int i;
    for (i = 0; i < 16; i++) {
        uint32_t m = p->w[i];
        while (m) {
            int pg = i * 32 + __builtin_ctz(m);
            m &= m - 1;
            last[pg] = seq;
        }
    }
}

static void print_stats(void) {
    int i;
    fprintf(stderr, "gs: %d workers, %llu commands, %llu states, %llu uploads queued (%.1f MB), %llu CLUTs from "
                    "them; waited %.3f s for room; waits:",
            nworkers, (unsigned long long)st_cmds, (unsigned long long)st_states, (unsigned long long)st_uploads,
            st_upload_bytes / 1048576.0, (unsigned long long)st_forward, st_full_ns / 1e9);
    for (i = 0; i < F_N; i++) {
        fprintf(stderr, " %s %llu (%.3f s)%s", flush_names[i], (unsigned long long)st_flush[i], st_wait_why[i] / 1e9,
                i + 1 < F_N ? "," : "");

    }
    fprintf(stderr, "; barriers:");
    for (i = 0; i < B_N; i++)
        fprintf(stderr, " %s %llu%s", barrier_names[i], (unsigned long long)st_barrier[i], i + 1 < B_N ? "," : "");
    fprintf(stderr, "; busy (s), of it at barriers:");
    for (i = 0; i < nworkers; i++)
        fprintf(stderr, " %.2f/%.2f", st_busy_ns[i] / 1e9, st_bar_ns[i] / 1e9);
    fputc(10, stderr);
}

/* A new state for the registers as they are now */
static void new_state(void) {
    if (nworkers > 0 && cur_used) {
        uint32_t next = (cur_slot + 1) % SCAP;
        if (state_last[next] != 0 && !reached(state_last[next])) {
            uint64_t t0 = SDL_GetTicksNS();
            publish();
            wait_for(state_last[next]);
            st_full_ns += SDL_GetTicksNS() - t0;
        }
        cur_slot = next;
        cur = &states[next];
        cur_used = 0;
    }
    gs_setup(cur);
    gs.dirty = 0;
    st_states++;
}

/* Append a command; the one QCAP back, whose slot this is, must be done */
static void enqueue(const GsDraw *d, uint32_t type, const GsVertex *v, int nv, int y0, int y1) {
    GsCmd *c;
    /* (the seqs kept per page and arena chunk are compared modulo 2^32) */
    if ((seq & 0x3FFFFFFF) == 0x3FFFFFFF)
        gs_flush();
    if (!reached(seq - QCAP + 1)) {
        uint64_t t0 = SDL_GetTicksNS();
        publish();
        wait_for(seq - QCAP + 1);
        st_full_ns += SDL_GetTicksNS() - t0;
    }
    c = &ring[seq & (QCAP - 1)];
    c->d = d;
    c->up = NULL;
    c->type = type;
    c->y0 = y0;
    c->y1 = y1;
    if (nv)
        memcpy(c->v, v, (size_t)nv * sizeof(GsVertex));
    seq++;
    st_cmds++;
    if (d) {
        state_last[cur_slot] = seq;
        cur_used = 1;
    }
    pending = 1;
}

/* Everything queued so far is done before anything queued after */
static void enqueue_barrier(int why) {
    if (stats)
        st_barrier[why]++;
    enqueue(NULL, CMD_BARRIER, NULL, 0, 0, 0);
    memset(&aw, 0, sizeof(aw));
    memset(&ar, 0, sizeof(ar));
    memset(&uw, 0, sizeof(uw));
}

/* An image upload the workers write (gs_mem.c, when the pages are still
   in use by the queue): u's data is copied */
void gs_queue_upload(const GsUpload *u, uint32_t bytes, const GsPages *dest) {
    GsUpload *q;
    uint32_t need = (uint32_t)((sizeof(GsUpload) + bytes + 15) & ~15u), c0, c1, ch, last = 0;
    GsCmd *c;
    int i;

    if (need > ARENA / 2) {
        /* can't happen with the GIF's 16-bit NLOOP; write it here */
        gs_flush();
        gs_upload_rows(u, &band_all);
        return;
    }
    /* Room in the arena, a ring: the uploads that used it must be done */
    if (arena_pos + need > ARENA)
        arena_pos = 0;
    c0 = arena_pos >> ACHUNK;
    c1 = (arena_pos + need - 1) >> ACHUNK;
    /* (only chunks entered anew: their last use was a lap ago) */
    for (ch = c0; ch <= c1; ch++)
        if ((int)ch != arena_open && arena_last[ch] && !reached(arena_last[ch]) &&
            (!last || (int32_t)(arena_last[ch] - last) > 0))
            last = arena_last[ch];
    arena_open = (int)c1;
    if (last) {
        uint64_t t0 = SDL_GetTicksNS();
        if (stats)
            st_flush[F_ROOM]++;
        publish();
        wait_for(last);
        st_full_ns += SDL_GetTicksNS() - t0;
    }
    if (pages_meet(dest, &aw) || pages_meet(dest, &ar) || pages_meet(dest, &uw))
        enqueue_barrier(B_UPLOAD);
    q = (GsUpload *)(arena + arena_pos);
    *q = *u;
    q->data = arena + arena_pos + sizeof(GsUpload);
    memcpy((uint8_t *)q->data, u->data, bytes);
    arena_pos += need;
    enqueue(NULL, CMD_UPLOAD, NULL, 0, 0, 0);
    c = &ring[(seq - 1) & (QCAP - 1)];
    c->up = q;
    for (ch = c0; ch <= c1; ch++)
        arena_last[ch] = seq;
    for (i = 0; i < 16; i++)
        uw.w[i] |= dest->w[i];
    pages_mark(page_w, dest);
    uphist[uphist_n % UPHIST].u = q;
    uphist[uphist_n % UPHIST].seq = seq;
    uphist_n++;
    st_uploads++;
    st_upload_bytes += bytes;
    publish();
}

/* The pages one buffer (frame or Z) of a draw covers in rows y0..y1-1,
   columns x0..x1-1, with their keys. Returns 0 if the buffer wraps onto
   itself there. */
typedef struct PrimPages {
    int n;
    uint16_t page[1024];
    uint32_t key[1024];
} PrimPages;

static int buffer_pages(PrimPages *pp, GsPages *mine, uint32_t bp, uint32_t bw, uint32_t lay, int ph, int x0,
                        int x1, int y0, int y1) {
    int r, c;
    if (bw == 0 || (uint32_t)x1 > bw * 64)
        return 0;
    for (r = y0 / ph; r <= (y1 - 1) / ph; r++)
        for (c = x0 / 64; c <= (x1 - 1) / 64; c++) {
            uint32_t p = ((bp >> 5) + (uint32_t)r * bw + (uint32_t)c) & 511;
            uint32_t key = (bp >> 5) | (bw << 9) | (lay << 15) | ((uint32_t)r << 18) | ((uint32_t)c << 25);
            if (pages_has(mine, p)) {
                int i;
                for (i = 0; i < pp->n; i++)
                    if (pp->page[i] == p && pp->key[i] != key)
                        return 0;
                continue;
            }
            if (pp->n == 1024)
                return 0;
            pages_add(mine, p);
            pp->page[pp->n] = (uint16_t)p;
            pp->key[pp->n++] = key;
        }
    return 1;
}

static int wrap_range(int mode, uint32_t size_log2, int32_t mn, int32_t mx, int32_t l, int32_t h, int32_t *a,
                      int32_t *b);

/* A sprite that reads, as a texture, pages it draws into (a copy of the
   frame onto itself, or the next buffer along). Drawing it on one thread,
   top to bottom, is what the GS does; on several threads it is the same
   if the rows that read those pages read only rows all drawn before them
   (or all drawn after them): those rows go after a barrier (or before
   one). Returns the row to split at, -1 if the rows can't be split, or
   -2 if no row reads what it draws after all. */
static int sprite_split(const GsDraw *d, const GsVertex *v, const GsPages *mine, const PrimPages *pp) {
    const GsTex *t = &d->t;
    int32_t r[6], rr[2], a0, a1, b0, b1;
    int y, i, dmin = 0x7FFFFFFF, dmax = -1, rmin = 0x7FFFFFFF, rmax = -1;

    if (!gs_sprite_texels(d, &v[0], &v[1], r))
        return -2;
    if (!wrap_range(t->wms, t->tw, t->minu, t->maxu, r[0], r[1], &a0, &a1))
        return -1;
    for (y = r[4]; y < r[5]; y++) {
        GsPages rp;
        gs_sprite_row_texels(d, &v[0], &v[1], y, rr);
        if (!wrap_range(t->wmt, t->th, t->minv, t->maxv, rr[0], rr[1], &b0, &b1))
            return -1;
        memset(&rp, 0, sizeof(rp));
        gs_pages_rect(&rp, t->psm, t->tbp, t->tbw, (uint32_t)a0, (uint32_t)b0, (uint32_t)(a1 - a0 + 1),
                      (uint32_t)(b1 - b0 + 1));
        if (!pages_meet(&rp, mine))
            continue;
        if (y < dmin)
            dmin = y;
        if (y > dmax)
            dmax = y;
        /* the rows of this sprite that draw into the pages row y reads */
        for (i = 0; i < pp->n; i++)
            if (pages_has(&rp, pp->page[i])) {
                int ph = ((pp->key[i] >> 15) & 7) >= 2 ? 64 : 32, pr = (int)((pp->key[i] >> 18) & 127);
                if (pr * ph < rmin)
                    rmin = pr * ph;
                if (pr * ph + ph - 1 > rmax)
                    rmax = pr * ph + ph - 1;
            }
    }
    if (dmax < 0)
        return -2;
    if (rmax < dmin)
        return dmin;
    if (rmin > dmax)
        return dmax + 1;
    return -1;
}

/* Texels l..h through a wrap mode: [*a, *b]; 0 if there is no useful
   bound */
static int wrap_range(int mode, uint32_t size_log2, int32_t mn, int32_t mx, int32_t l, int32_t h, int32_t *a,
                      int32_t *b) {
    int32_t mask = (1 << size_log2) - 1;
    switch (mode) {
    case 0:
        if ((l >> size_log2) != (h >> size_log2)) {
            l = 0;
            h = mask;
        } else {
            l &= mask;
            h &= mask;
        }
        break;
    case 1:
        l = l < 0 ? 0 : l > mask ? mask : l;
        h = h < 0 ? 0 : h > mask ? mask : h;
        break;
    case 2:
        if (mn > mx)
            return 0;
        l = l < mn ? mn : l > mx ? mx : l;
        h = h < mn ? mn : h > mx ? mx : h;
        break;
    default:
        return 0;
    }
    *a = l;
    *b = h;
    return 1;
}

/* The texels that sampling at coordinates lo..hi (in texels, known only
   to within rounding) can read, nearest or bilinear */
static int tex_range(int mode, uint32_t size_log2, int32_t mn, int32_t mx, double lo, double hi, int32_t *a,
                     int32_t *b) {
    double e;
    if (!(lo > -65536.0 && hi < 65536.0))
        return 0;
    e = 0.0625 + (hi - lo) / 4096.0;
    return wrap_range(mode, size_log2, mn, mx, (int32_t)floor(lo - e - 0.5), (int32_t)floor(hi + e - 0.5) + 1, a,
                      b);
}

/* The pages a textured draw can read. A sprite's are worked out exactly
   (gs_sprite_texels); otherwise the hull of the vertices' coordinates
   bounds them (interpolation, perspective-correct too while every Q has
   the same sign, stays inside it). 0: no bound better than the whole
   texture's (d->rtex). */
static int prim_tex_pages(const GsDraw *d, const GsVertex *v, int nv, uint32_t type, GsPages *out) {
    const GsTex *t = &d->t;
    double ul = 1e30, uh = -1e30, vl = 1e30, vh = -1e30;
    int32_t a0, a1, b0, b1;
    int i;

    if (type == 6) {
        int32_t r[6];
        if (!gs_sprite_texels(d, &v[0], &v[1], r)) {
            memset(out, 0, sizeof(*out));
            return 1;
        }
        if (!wrap_range(t->wms, t->tw, t->minu, t->maxu, r[0], r[1], &a0, &a1) ||
            !wrap_range(t->wmt, t->th, t->minv, t->maxv, r[2], r[3], &b0, &b1))
            return 0;
    } else {
        for (i = 0; i < nv; i++) {
            double u, w;
            if (d->fst) {
                u = v[i].u / 16.0;
                w = v[i].v / 16.0;
            } else {
                double q = v[i].q;
                if (!(q > 0.0) && !(q < 0.0))
                    return 0;
                if ((q > 0.0) != (v[0].q > 0.0))
                    return 0;
                u = (double)v[i].s / q * (double)(1 << t->tw);
                w = (double)v[i].t / q * (double)(1 << t->th);
            }
            if (u < ul) ul = u;
            if (u > uh) uh = u;
            if (w < vl) vl = w;
            if (w > vh) vh = w;
        }
        if (!tex_range(t->wms, t->tw, t->minu, t->maxu, ul, uh, &a0, &a1) ||
            !tex_range(t->wmt, t->th, t->minv, t->maxv, vl, vh, &b0, &b1))
            return 0;
    }
    memset(out, 0, sizeof(*out));
    gs_pages_rect(out, t->psm, t->tbp, t->tbw, (uint32_t)a0, (uint32_t)b0, (uint32_t)(a1 - a0 + 1),
                  (uint32_t)(b1 - b0 + 1));
    return 1;
}

static const int prim_nverts[8] = {1, 2, 2, 3, 3, 3, 2, 0};

void gs_draw_prim(const GsVertex *v, uint32_t type) {
    static PrimPages pp;
    GsPages mine, fpages, tp;
    const GsPages *rt = NULL;
    const GsDraw *d;
    int nv = prim_nverts[type & 7], i, x0, x1, y0, y1, solo = 0, split = -2, why = -1;
    int32_t mnx, mxx, mny, mxy;

    if (nworkers < 0)
        gs_queue_init();
    if (gs.dirty)
        new_state();
    d = cur;
    if (!d->fvalid || nv == 0)
        return;

    /* The rows and columns it can touch, within the scissor */
    mnx = mxx = v[0].x;
    mny = mxy = v[0].y;
    for (i = 1; i < nv; i++) {
        if (v[i].x < mnx) mnx = v[i].x;
        if (v[i].x > mxx) mxx = v[i].x;
        if (v[i].y < mny) mny = v[i].y;
        if (v[i].y > mxy) mxy = v[i].y;
    }
    x0 = mnx >> 4;
    x1 = ((mxx + 15) >> 4) + 1;
    y0 = mny >> 4;
    y1 = ((mxy + 15) >> 4) + 1;
    if (x0 < d->scx0) x0 = d->scx0;
    if (x1 > d->scx1 + 1) x1 = d->scx1 + 1;
    if (y0 < d->scy0) y0 = d->scy0;
    if (y1 > d->scy1 + 1) y1 = d->scy1 + 1;
    if (x0 >= x1 || y0 >= y1)
        return;

    if (nworkers == 0) {
        gs_render(d, &band_all, v, type);
        return;
    }

    /* What it reads and writes */
    if (d->tme)
        rt = prim_tex_pages(d, v, nv, type, &tp) ? &tp : &d->rtex;
    memset(&mine, 0, sizeof(mine));
    pp.n = 0;
    if (!buffer_pages(&pp, &mine, d->fbp, d->fbw, d->f16 ? 2 + d->flay : d->flay, d->f16 ? 64 : 32, x0, x1, y0, y1))
        solo = 1;
    fpages = mine;
    if (!solo && d->zon) {
        GsPages zp;
        int nf = pp.n;
        memset(&zp, 0, sizeof(zp));
        if (!buffer_pages(&pp, &zp, d->zbp, d->fbw, d->z16 ? 2 + d->zlay : 1, d->z16 ? 64 : 32, x0, x1, y0, y1))
            solo = 1;
        /* frame and Z over the same memory, addressed differently */
        for (i = nf; !solo && i < pp.n; i++) {
            int j;
            if (!pages_has(&fpages, pp.page[i]))
                continue;
            for (j = 0; j < nf; j++)
                if (pp.page[j] == pp.page[i] && pp.key[j] != pp.key[i])
                    solo = 1;
        }
        for (i = 0; i < 16; i++)
            mine.w[i] |= zp.w[i];
    }
    if (!solo && rt && pages_meet(rt, &mine)) {
        split = type == 6 ? sprite_split(d, v, &mine, &pp) : -1;
        if (split == -1)
            solo = 1;
    }

    /* ... against what is queued */
    if (rt && (pages_meet(rt, &aw) || pages_meet(rt, &uw)))
        why = B_TEX;
    else if (pages_meet(&mine, &ar) || pages_meet(&mine, &uw))
        why = B_WRITE;
    for (i = 0; why < 0 && i < pp.n; i++)
        if (pages_has(&aw, pp.page[i]) && wkey[pp.page[i]] != pp.key[i])
            why = B_WRITE;
    if (why >= 0)
        enqueue_barrier(why);

    if (solo) {
        if (stats)
            st_barrier[B_SOLO]++;
        enqueue(NULL, CMD_BARRIER, NULL, 0, 0, 0);
        enqueue(d, type | CMD_SOLO, v, nv, y0, y1);
        enqueue_barrier(B_SOLO);
    } else if (split > y0 && split < y1) {
        enqueue(d, type, v, nv, y0, split);
        enqueue_barrier(B_SPLIT);
        enqueue(d, type, v, nv, split, y1);
    } else {
        enqueue(d, type, v, nv, y0, y1);
    }
    /* (a draw alone is done before anything after it) */
    for (i = 0; !solo && i < pp.n; i++) {
        pages_add(&aw, pp.page[i]);
        wkey[pp.page[i]] = pp.key[i];
    }
    for (i = 0; rt && !solo && i < 16; i++)
        ar.w[i] |= rt->w[i];
    pages_mark(page_w, &mine);
    if (rt)
        pages_mark(page_r, rt);
    publish();
}
