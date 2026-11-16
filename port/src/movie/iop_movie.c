/*
 * GS_IOP.IRX's movie side, on the EE (grx_movie.h): commands 0x300/0x310
 * and movie_server.
 *
 * MovPlay (4633B0) opens the .MOV twice, on stream 0 ("DR", the whole
 * file is read into it with 0x101) and stream 1 ("PSS", nothing read into
 * it), and sends 0x300 (DR, PSS, VBS = 2). On the IOP, 0x300 records the
 * three ring numbers and sets movflag 1; movie_server then runs in the
 * IOP's main loop and moves the file one sector at a time out of the DR
 * ring (releasing it there) into the PSS ring or into ring 2, which the
 * sound driver plays as VBS stream 0 (0x208 with no data prepared it,
 * 0x218 starts it). movflag goes to the status block's movieState
 * (+0x2C) every round: 1 waiting for the header, 2 the header read (the
 * PSS ring's +0x1C gets the PSS byte count, which MovPlay passes to
 * MovReadLoop) and the first VBS chunk going, 3 a PSS chunk, 4 a VBS
 * chunk. MovPlay waits for >= 2; MovReadLoop starts the display at >= 3.
 * 0x310 sets movflag 0.
 *
 * Here: the DR stream is cdvd.c's ring. Its disc sectors are known from
 * the 0x101 commands, which grx_movie_command sees before cdvd.c does.
 * A PSS sector is released from DR (grx_gsiop_strm_take) and queued into
 * the PSS ring as a disc read (a 0x101 to cdvd.c), so the EE's fetches
 * read it from the disc image when they come. A VBS sector is taken from
 * DR with its data and given to the sound driver (grx_snd_strm_feed).
 *
 * How far ahead: the IOP's rings were small; cdvd.c's are virtual (32 MB
 * a ring) and the sound driver's read-ahead holds 256 sectors (about 9 s
 * of the movies' audio). So the PSS ring is kept at most PSS_AHEAD
 * sectors ahead of the EE (several chunks: the next VBS chunk is always
 * within reach), and a VBS sector the sound driver refuses waits. If the
 * EE has run out of video while the audio is full (a sound driver that
 * doesn't drain), the VBS sector is dropped rather than stopping the
 * movie.
 *
 * Plain C over cdvd.c, disc.c and the sound driver; no game names.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "grx_movie.h"
#include "grx_disc.h"
#include "grx_snd.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

#define SECTOR 2048
#define SLOTS 4
#define PSS_AHEAD 2048       /* sectors (4 MB, about 5 s of video) */

/* The status block (SndIopStatus, include/snd.h) */
#define ST_DONE_SEQ 0x0E
#define ST_MOVIE_STATE 0x2C
#define ST_STRM 0x80
#define STRM_SIZE 0x20
#define STRM_SEC_NUM 0x02
#define STRM_TOTAL 0x1C

/* The disc runs a stream ring holds (from its 0x101s) and how many of its
   sectors have been released, so the sector at the ring's front is known */
typedef struct {
    uint32_t lsn, count;
} Run;

typedef struct {
    Run run[32];
    int nrun;
    uint32_t head;           /* sectors released since the slot was opened */
} SlotRuns;

static SlotRuns slots[SLOTS];

static struct {
    int flag;                /* movflag: 0 off, 1 header, 2-4 moving */
    int dr, pss, vbs;
    GrxMovSplit split;
    int have_vbs_sector;     /* a VBS sector taken from DR, not yet accepted */
    uint8_t vbs_sector[SECTOR];
    int dropped;
    int in_poll;
} mv;

static uint8_t *status(void) {
    return grx_gsiop_status();
}

static int16_t st16(int off) {
    int16_t v;
    memcpy(&v, status() + off, 2);
    return v;
}

static void put32(int off, int32_t v) {
    memcpy(status() + off, &v, 4);
}

static int strm_sec_num(int slot) {
    return st16(ST_STRM + slot * STRM_SIZE + STRM_SEC_NUM);
}

static void slot_reset(int slot) {
    if (slot >= 0 && slot < SLOTS) {
        memset(&slots[slot], 0, sizeof(slots[slot]));
    }
}

/* The disc sector pos sectors after the front of a slot's ring, or -1 */
static int64_t slot_lsn(int slot, uint32_t pos) {
    const SlotRuns *s = &slots[slot];
    int i;

    pos += s->head;
    for (i = 0; i < s->nrun; i++) {
        if (pos < s->run[i].count) {
            return (int64_t)s->run[i].lsn + pos;
        }
        pos -= s->run[i].count;
    }
    return -1;
}

/* Drops the runs wholly released */
static void slot_trim(int slot) {
    SlotRuns *s = &slots[slot];

    while (s->nrun > 0 && s->head >= s->run[0].count) {
        s->head -= s->run[0].count;
        memmove(s->run, s->run + 1, (size_t)(s->nrun - 1) * sizeof(Run));
        s->nrun--;
    }
}

static void slot_release(int slot, uint32_t n) {
    if (slot >= 0 && slot < SLOTS) {
        slots[slot].head += n;
        slot_trim(slot);
    }
}

/* movflag as the IOP leaves it after its last sector: a chunk that is
   done has already turned the state over */
static int movie_state(void) {
    if (mv.flag < 2) {
        return mv.flag;
    }
    if (mv.split.left > 0) {
        return mv.split.state;
    }
    return mv.split.state == 3 ? 4 : 3;
}

/* One sector of DR into a 0x101 on the PSS ring */
static int move_pss(void) {
    int64_t lsn = slot_lsn(mv.dr, 0);

    if (lsn < 0 || grx_gsiop_strm_take(mv.dr, 1, NULL) != 1) {
        return 0;
    }
    slot_release(mv.dr, 1);
    /* as a command, so cdvd.c keeps its ring (and doneSeq stays) */
    grx_gsiop_command((uint16_t)st16(ST_DONE_SEQ), 0x101, mv.pss, (int32_t)lsn, 1);
    return 1;
}

/* One sector of DR to the sound driver. 0 if it is full. */
static int move_vbs(void) {
    if (!mv.have_vbs_sector) {
        if (grx_gsiop_strm_take(mv.dr, 1, mv.vbs_sector) != 1) {
            return 0;
        }
        slot_release(mv.dr, 1);
        mv.have_vbs_sector = 1;
    }
    if (grx_snd_strm_feed(mv.vbs - 2, mv.vbs_sector, 1) == 1) {
        mv.have_vbs_sector = 0;
        return 1;
    }
    if (strm_sec_num(mv.pss) == 0) {
        /* the EE is out of video and the audio won't take more */
        if (mv.dropped++ == 0) {
            grx_log("movie: the sound driver takes no more of the audio stream; audio dropped");
        }
        mv.have_vbs_sector = 0;
        return 1;
    }
    return 0;
}

void grx_movie_poll(void) {
    int budget = 4096;

    if (mv.flag == 0 || mv.in_poll) {
        return;
    }
    mv.in_poll = 1;
    if (mv.flag == 1) {
        /* the header, from the first sector of the file (DR's front),
           once the ring has some data (the IOP waits for 3 sectors) */
        uint8_t hdr[SECTOR];
        int64_t lsn = slot_lsn(mv.dr, 0);
        if (lsn >= 0 && strm_sec_num(mv.dr) >= 3 &&
            grx_disc_read_sectors(grx_cdvd_disc(), (uint32_t)lsn, 1, hdr) == 1) {
            grx_mov_split_init(&mv.split, hdr);
            put32(ST_STRM + mv.pss * STRM_SIZE + STRM_TOTAL, mv.split.pss_bytes);
            mv.flag = 2;
        }
    }
    while (mv.flag >= 2 && budget-- > 0) {
        int kind;
        GrxMovSplit save;

        if (mv.have_vbs_sector) {
            if (!move_vbs()) {
                break;
            }
            continue;
        }
        if (strm_sec_num(mv.dr) <= 0) {
            break;
        }
        save = mv.split;
        kind = grx_mov_split_next(&mv.split);
        if (kind == 0) {
            if (strm_sec_num(mv.pss) >= PSS_AHEAD || !move_pss()) {
                mv.split = save;
                break;
            }
        } else if (!move_vbs() && !mv.have_vbs_sector) {
            mv.split = save;
            break;
        }
    }
    if (mv.flag >= 1) {
        put32(ST_MOVIE_STATE, movie_state());
    }
    mv.in_poll = 0;
}


/* GRX_MOVIE_TRACE=1: the movie commands and the decoder's steps in the log */
static int movie_trace(void) {
    static int on = -1;
    if (on < 0) {
        const char *e = getenv("GRX_MOVIE_TRACE");
        on = e != NULL && *e != 0 && *e != '0';
    }
    return on;
}

int grx_movie_command(uint32_t cmd, const void *args, uint32_t size) {
    int32_t a[3] = {0, 0, 0};

    if (args != NULL) {
        memcpy(a, args, size < sizeof(a) ? size : sizeof(a));
    }
    if (movie_trace()) {
        grx_log("movie: command 0x%03X (%d, %d, %d)", cmd & 0xFFF, a[0], a[1], a[2]);
    }
    switch (cmd & 0xFFF) {
    case 0x100:                 /* the stream side's reset */
        memset(slots, 0, sizeof(slots));
        return 0;
    case 0x101:                 /* (slot, lsn, sectors) */
        if (a[0] >= 0 && a[0] < SLOTS && a[2] > 0) {
            SlotRuns *s = &slots[a[0]];
            Run *last = s->nrun > 0 ? &s->run[s->nrun - 1] : NULL;
            if (last != NULL && last->lsn + last->count == (uint32_t)a[1]) {
                last->count += (uint32_t)a[2];
            } else if (s->nrun < (int)(sizeof(s->run) / sizeof(s->run[0]))) {
                s->run[s->nrun].lsn = (uint32_t)a[1];
                s->run[s->nrun].count = (uint32_t)a[2];
                s->nrun++;
            }
        }
        return 0;
    case 0x110:                 /* (slot, sectors): released by the EE */
        slot_release(a[0], a[1] > 0 ? (uint32_t)a[1] : 0);
        return 0;
    case 0x120:                 /* (slot): closed */
        slot_reset(a[0]);
        return 0;
    case 0x300:                 /* (DR, PSS, VBS) */
        if (a[0] < 0 || a[0] >= SLOTS || a[1] < 0 || a[1] >= SLOTS || a[2] < 2 || a[2] >= SLOTS ||
            a[0] == a[1]) {
            /* MovPlayList's (0, 0, 2) form has no file to split */
            grx_log("movie: 0x300 (%d, %d, %d) not supported", a[0], a[1], a[2]);
            return 1;
        }
        memset(&mv, 0, sizeof(mv));
        mv.dr = a[0];
        mv.pss = a[1];
        mv.vbs = a[2];
        mv.flag = 1;
        put32(ST_MOVIE_STATE, 1);
        grx_movie_poll();
        return 1;
    case 0x310:                 /* stop */
        mv.flag = 0;
        mv.have_vbs_sector = 0;
        put32(ST_MOVIE_STATE, 0);
        return 1;
    default:
        return 0;
    }
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h): not during a movie (its decoder is this
   run's); otherwise the stream runs the IOP side keeps */
int grx_mpeg_active(void);

const char *grx_movie_state_blocker(void) {
    return mv.flag != 0 || grx_mpeg_active() ? "a movie is playing" : NULL;
}

void grx_movie_state_io(GrxState *s) {
    GRX_STATE_VAR(s, slots);
    GRX_STATE_VAR(s, mv);
}
