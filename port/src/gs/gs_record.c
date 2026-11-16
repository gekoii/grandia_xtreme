/* Recording what the GS is fed, for port/tests/gs_replay.c: every call
   into the GS API (grx_gs.h) with its data, so a stretch of the game can
   be rendered again without the game, to check that the renderer's output
   stays the same and to time it.

   GRX_GS_RECORD=file turns it on. GRX_GS_RECORD_FROM=n starts at the n-th
   vsync (default 1) with a snapshot of the GS (local memory, registers,
   CLUT, transfers and GIF paths); GRX_GS_RECORD_TO=n stops after the n-th.
   Recordings hold game data: keep them under build/.

   The format (little-endian), after the 8-byte magic "GSREC001":
     'S' u32 vsync, u32 n, then n bytes of state (gs_rec_save_state),
         then the 4 MB of local memory
     'G' u32 qwc, u32 path, qwc * 16 bytes      grx_gs_gif
     'R'                                        grx_gs_reset
     'X'                                        grx_gs_gif_reset
     'I' u32 qwc, qwc * 16 bytes                grx_gs_read_image (what it gave)
     'V' u32 vsync, GS_REC_PRIV bytes           grx_gs_vsync (the privileged
                                                registers just before it)
     'E'                                        the end */
#include <stdio.h>
#include <stdlib.h>
#include "gs_internal.h"
#include "grx_gs.h"
#include "gs_draw.h"
#include "grx_state_parts.h"
#include "grx_platform.h"

int gs_rec_active;

static FILE *rec;
static int rec_state = -1; /* -1: env not read, 0: off, 1: waiting, 2: on, 3: done */
static uint32_t rec_from = 1, rec_to = 0xFFFFFFFFu, vsyncs;

static void put32(uint32_t v) {
    fwrite(&v, 4, 1, rec);
}

static void rec_init(void) {
    const char *f = getenv("GRX_GS_RECORD"), *e;
    rec_state = 0;
    if (f == NULL || !*f)
        return;
    if ((e = getenv("GRX_GS_RECORD_FROM")) != NULL && atoi(e) > 0)
        rec_from = (uint32_t)atoi(e);
    if ((e = getenv("GRX_GS_RECORD_TO")) != NULL && atoi(e) > 0)
        rec_to = (uint32_t)atoi(e);
    rec = fopen(f, "wb");
    if (rec == NULL) {
        fprintf(stderr, "gs: can't write %s\n", f);
        return;
    }
    setvbuf(rec, NULL, _IOFBF, 1 << 22);
    fwrite("GSREC001", 8, 1, rec);
    rec_state = 1;
}

static void rec_close(void) {
    if (rec != NULL) {
        fputc('E', rec);
        fclose(rec);
        rec = NULL;
    }
    gs_rec_active = 0;
    rec_state = 3;
}

void gs_rec_gif(const void *data, uint32_t qwc, int path) {
    fputc('G', rec);
    put32(qwc);
    put32((uint32_t)path);
    fwrite(data, 16, qwc, rec);
}

void gs_rec_event(int what) {
    fputc(what, rec);
}

void gs_rec_read_image(const void *data, uint32_t qwc) {
    fputc('I', rec);
    put32(qwc);
    fwrite(data, 16, qwc, rec);
}

/* Called first thing in grx_gs_vsync */
void gs_rec_vsync(void) {
    if (rec_state < 0)
        rec_init();
    if (rec_state == 0 || rec_state == 3)
        return;
    vsyncs++;
    if (rec_state == 1) {
        static uint8_t st[GS_REC_STATE_MAX];
        uint32_t n;
        if (vsyncs < rec_from)
            return;
        gs_flush();
        if (gs_backend) {
            /* local memory as the renderer has it */
            GsPages all;
            memset(&all, 0xFF, sizeof(all));
            gs_backend->reading(&all);
        }
        n = gs_rec_save_state(st);
        fputc('S', rec);
        put32(vsyncs);
        put32(n);
        fwrite(st, 1, n, rec);
        fwrite(gs_vram, 1, GS_VRAM_SIZE, rec);
        rec_state = 2;
        gs_rec_active = 1;
        fprintf(stderr, "gs: recording from vsync %u\n", vsyncs);
    }
    fputc('V', rec);
    put32(vsyncs);
    fwrite(gs.priv, 1, GS_REC_PRIV, rec);
    if (vsyncs >= rec_to) {
        fprintf(stderr, "gs: recording ends at vsync %u\n", vsyncs);
        rec_close();
    }
}

/* The state a snapshot holds, field by field, so recordings stay readable
   when GsState changes */
#define PUT(v) (memcpy(p, &(v), sizeof(v)), p += sizeof(v))
#define GET(v) (memcpy(&(v), p, sizeof(v)), p += sizeof(v))

static uint8_t *ctx_io(uint8_t *p, GsContext *c, int save) {
    uint64_t *f[12] = {&c->tex0, &c->clamp, &c->tex1, &c->xyoffset, &c->miptbp1, &c->miptbp2,
                       &c->scissor, &c->alpha, &c->test, &c->fba, &c->frame, &c->zbuf};
    int i;
    for (i = 0; i < 12; i++) {
        if (save)
            PUT(*f[i]);
        else
            GET(*f[i]);
    }
    return p;
}

static uint8_t *trx_io(uint8_t *p, GsTransfer *t, int save) {
    int32_t a = t->active;
    uint32_t *f[10] = {&t->bp, &t->bw, &t->psm, &t->sx, &t->x, &t->y, &t->w, &t->h, &t->left, &t->npend};
    int i;
    if (save)
        PUT(a);
    else
        GET(a);
    t->active = a;
    for (i = 0; i < 10; i++) {
        if (save)
            PUT(*f[i]);
        else
            GET(*f[i]);
    }
    if (save)
        PUT(t->pend);
    else
        GET(t->pend);
    return p;
}

static uint8_t *vtx_io(uint8_t *p, GsVertex *v, int save) {
    if (save) {
        PUT(v->x); PUT(v->y); PUT(v->z); PUT(v->rgba); PUT(v->s); PUT(v->t); PUT(v->q);
        PUT(v->u); PUT(v->v); PUT(v->f);
    } else {
        GET(v->x); GET(v->y); GET(v->z); GET(v->rgba); GET(v->s); GET(v->t); GET(v->q);
        GET(v->u); GET(v->v); GET(v->f);
    }
    return p;
}

/* A loaded state's counters index arrays: a damaged or foreign one is
   brought back in range rather than trusted. The vertex queue holds 3;
   an upload's split pixel is shorter than one pixel of its format (none
   for 4-bit, which never splits), a readback's at most pend's 8 bytes; a
   GIF tag's NREG 0 means 16, and the register being fed is one of them */
static void load_sanitize(void) {
    static const uint32_t up_bpp[8] = {4, 3, 2, 1, 0, 1, 0, 0}; /* by GS_K* kind, as trx_bits */
    int i;
    if (gs.nv < 0 || gs.nv > 2)
        gs.nv = 0;
    if (gs.up.npend >= up_bpp[gs_psm[gs.up.psm & 63].kind & 7])
        gs.up.npend = 0;
    if (gs.down.npend > 8)
        gs.down.npend = 0;
    for (i = 0; i < 4; i++) {
        GsGifPath *g = &gs.path[i];
        if (g->nreg == 0 || g->nreg > 16)
            g->nreg = 16;
        if (g->curreg >= g->nreg)
            g->curreg = 0;
    }
}

static uint32_t state_io(uint8_t *buf, int save) {
    uint8_t *p = buf;
    uint64_t *r[19] = {&gs.prim, &gs.prmode, &gs.prmodecont, &gs.rgbaq, &gs.st, &gs.uv, &gs.fog,
                       &gs.texclut, &gs.scanmsk, &gs.texa, &gs.fogcol, &gs.dimx, &gs.dthe, &gs.colclamp,
                       &gs.pabe, &gs.bitbltbuf, &gs.trxpos, &gs.trxreg, &gs.trxdir};
    int32_t nv = gs.nv;
    int i;

    for (i = 0; i < 19; i++) {
        if (save)
            PUT(*r[i]);
        else
            GET(*r[i]);
    }
    for (i = 0; i < 2; i++)
        p = ctx_io(p, &gs.ctx[i], save);
    if (save) {
        PUT(gs.q); PUT(nv); PUT(gs.clut); PUT(gs.cbp0); PUT(gs.cbp1);
    } else {
        GET(gs.q); GET(nv); GET(gs.clut); GET(gs.cbp0); GET(gs.cbp1);
        gs.nv = nv;
    }
    for (i = 0; i < 3; i++)
        p = vtx_io(p, &gs.vq[i], save);
    p = trx_io(p, &gs.up, save);
    p = trx_io(p, &gs.down, save);
    for (i = 0; i < 4; i++) {
        GsGifPath *g = &gs.path[i];
        if (save) {
            PUT(g->nloop); PUT(g->nreg); PUT(g->curreg); PUT(g->flg); PUT(g->regs);
        } else {
            GET(g->nloop); GET(g->nreg); GET(g->curreg); GET(g->flg); GET(g->regs);
        }
    }
    if (save)
        memcpy(p, gs.priv, GS_REC_PRIV_STATE);
    else
        memcpy(gs.priv, p, GS_REC_PRIV_STATE);
    p += GS_REC_PRIV_STATE;
    if (!save) {
        load_sanitize();
        gs.dirty = 1;
    }
    return (uint32_t)(p - buf);
}

uint32_t gs_rec_save_state(uint8_t *buf) {
    return state_io(buf, 1);
}

uint32_t gs_rec_load_state(const uint8_t *buf) {
    return state_io((uint8_t *)buf, 0);
}

/* A snapshot back in place (the replayer) */
void gs_rec_load(const uint8_t *state, const uint8_t *vram) {
    gs_flush();
    gs_rec_load_state(state);
    memcpy(gs_vram, vram, GS_VRAM_SIZE);
    if (gs_backend)
        gs_backend->loaded();
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h): the registers as the recorder keeps
   them, and local memory as the renderer has it (render targets read
   back). After a load the renderer starts again from local memory. */
const char *grx_gs_state_blocker(void) {
    return gs.up.active || gs.down.active ? "a GS transfer is in progress" : NULL;
}

void grx_gs_state_io(GrxState *s) {
    static uint8_t st[GS_REC_STATE_MAX];
    uint32_t n;
    gs_flush();
    if (!grx_state_loading(s)) {
        if (gs_backend) {
            GsPages all;
            memset(&all, 0xFF, sizeof(all));
            gs_backend->reading(&all);
        }
        n = gs_rec_save_state(st);
        GRX_STATE_VAR(s, n);
        grx_state_io(s, st, n);
        grx_state_io(s, gs_vram, GS_VRAM_SIZE);
        return;
    }
    GRX_STATE_VAR(s, n);
    if (n > sizeof(st)) {
        grx_fatal("gs: a save state's GS registers are too big (%u bytes)", n);
    }
    grx_state_io(s, st, n);
    gs_rec_load_state(st);
    grx_state_io(s, gs_vram, GS_VRAM_SIZE);
}

void grx_gs_state_loaded(void) {
    gs_ws_forget();
    if (gs_backend) {
        gs_backend->loaded();
    }
}
