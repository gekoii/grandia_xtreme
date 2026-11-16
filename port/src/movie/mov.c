/*
 * The .MOV interleave (grx_movie.h): which sectors are video (PSS) and
 * which audio (VBS). This is GS_IOP.IRX's movie_server (IRX .text
 * 0x94C-0x1288, in the module's own symbols), whose state 1 reads the
 * header and states 2-4 move one sector at a time from the file's ring to
 * the PSS or the VBS ring:
 *
 *   state 1: header (first sector of the file): the totals, intPSS,
 *            intVBS (negative: 0), time; if 1 <= time <= 4096,
 *            v1st = 65, the remainders from +0x110 and the last-chunk
 *            count from +0x118; else v1st = +0x110, no remainders,
 *            time 1. pss_add = pss_rem, vbs_add = vbs_rem,
 *            vbs_2nd = time_cnt & 1, time_cnt = (time_cnt >> 1) - 1
 *   state 2: v1st sectors to VBS (the header sector is the first)
 *   state 3: PSS sectors until intPSS + pss_add / time have gone,
 *            then pss_add = pss_add % time + pss_rem
 *   state 4: VBS sectors until intVBS + vbs_add / time have gone (then
 *            vbs_add = vbs_add % time + vbs_rem, time_cnt--); when
 *            vbs_2nd and time_cnt == 0, intVBS exactly. intVBS 0: none.
 *
 * Checked against all 16 movies on the disc: the PSS sectors it picks
 * make one unbroken program stream that ends exactly at the header's PSS
 * byte count (its end code), and the VBS sectors add up to the VBS count.
 */
#include <stdint.h>
#include <string.h>
#include "grx_movie.h"

static int32_t rd32(const uint8_t *p) {
    return (int32_t)((uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24);
}

static void next_pss(GrxMovSplit *s) {
    s->state = 3;
    s->left = s->int_pss + s->pss_add / s->time;
    s->pss_add = s->pss_add % s->time + s->pss_rem;
}

/* The next VBS chunk's length; 0 if there is none */
static void next_vbs(GrxMovSplit *s) {
    s->state = 4;
    if (s->int_vbs == 0) {
        s->vbs_add = s->vbs_add % s->time + s->vbs_rem;
        s->time_cnt--;
        s->left = 0;
    } else if (s->vbs_2nd && s->time_cnt == 0) {
        s->left = s->int_vbs;
    } else {
        s->left = s->int_vbs + s->vbs_add / s->time;
        s->vbs_add = s->vbs_add % s->time + s->vbs_rem;
        s->time_cnt--;
    }
}

void grx_mov_split_init(GrxMovSplit *s, const uint8_t *hdr) {
    memset(s, 0, sizeof(*s));
    s->pss_bytes = rd32(hdr + 0x100);
    s->vbs_bytes = rd32(hdr + 0x104);
    s->int_pss = rd32(hdr + 0x108);
    s->int_vbs = rd32(hdr + 0x10C);
    if (s->int_vbs < 0) {
        s->int_vbs = 0;
    }
    s->time = rd32(hdr + 0x114);
    if (s->time > 0 && s->time <= 4096) {
        int32_t x = rd32(hdr + 0x110);
        s->v1st = 65;
        s->pss_rem = (int32_t)((uint32_t)x >> 16);
        s->vbs_rem = (int16_t)(x & 0xFFFF);
        if (s->vbs_rem < 0) {
            s->vbs_rem = 0;
        }
        s->time_cnt = rd32(hdr + 0x118);
    } else {
        s->v1st = rd32(hdr + 0x110);
        s->pss_rem = s->vbs_rem = 0;
        s->time = 1;
        s->time_cnt = 0;
    }
    s->pss_add = s->pss_rem;
    s->vbs_add = s->vbs_rem;
    s->vbs_2nd = s->time_cnt & 1;
    s->time_cnt = (s->time_cnt >> 1) - 1;
    s->state = 2;
    s->left = s->v1st;
}

int grx_mov_split_next(GrxMovSplit *s) {
    /* chunks that are empty pass at once (a VBS chunk of 0 with intVBS 0) */
    while (s->left <= 0) {
        if (s->state == 3) {
            next_vbs(s);
        } else {
            next_pss(s);
        }
        if (s->left <= 0 && s->state == 3 && s->int_pss <= 0) {
            /* no PSS at all: everything is VBS */
            s->state = 4;
            s->left = 0x7FFFFFFF;
        }
    }
    s->left--;
    return s->state == 3 ? 0 : 1;
}
