/* The VU1 program's triangle stages: the depth-sort keys, the sort, packet
   assembly with ADC culling, and XGKICK. See vu1_internal.h and
   port/docs/vu.md.

   The triangle list is at 0x17F, one quadword per triangle: x, y, z the
   vertex indices (as they are from the VU0 paths; times 16, 12.4, from the
   VU1 paths), w free for the sort key. */
#include "vu1_internal.h"
#include "grx_gs.h"

#pragma STDC FP_CONTRACT OFF

/* The three vertex indices of a triangle */
static void tri_indices(uint32_t t, int vu1_indices, uint32_t idx[3]) {
    int k;
    if (vu1_indices) {
        /* itof4, ftoi0, mtir */
        V4 q = vu1_ld(VU1_TRI + t);
        for (k = 0; k < 3; k++) {
            idx[k] = (uint32_t)vu_ftoi(vu_itof(q.i[k], 4), 0) & 0xFFFFu;
        }
    } else {
        /* ilw.x/y/z */
        for (k = 0; k < 3; k++) {
            idx[k] = vu1_ilw(VU1_TRI + t, k);
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Sort keys (sub_2A50 for the VU0 paths, sub_2B10 for the VU1 paths): each
   triangle's key is the sum of its vertices' integer Z (as floats), stored
   as a float in the triangle's w. vf06.w gets the keys' sum and vf07.w
   their count, for the sort's mean. The caller has checked that there is
   more than one triangle. */

static void sort_keys(int vu1_indices) {
    V4 *vf = grx_vu1.vf;
    uint32_t ntri = vu1_ntri(), t, idx[3];
    int i;

    for (i = 0; i < 4; i++) {
        vf[VF_ZERO].f[i] = vf[VF_ZERO].f[i] * 0.0f; /* mulx vf31, vf31, vf00x */
        vf[VF_KEYSUM].f[i] = vf[VF_KEYSUM].f[i] * 0.0f;
    }
    vf[VF_KEYCNT].f[W] = vu_itof((int16_t)ntri, 0); /* mfir.w sign-extends, itof0 */
    for (t = 0; t < ntri; t++) {
        V4 key;
        float z0, z1, z2, acc;
        tri_indices(t, vu1_indices, idx);
        z0 = vu_itof(vu1_ld(VU1_POS + idx[0]).i[Z], 0);
        z1 = vu_itof(vu1_ld(VU1_POS + idx[1]).i[Z], 0);
        z2 = vu_itof(vu1_ld(VU1_POS + idx[2]).i[Z], 0);
        acc = 1.0f * z0;          /* mulaz.w by vf00w */
        acc = acc + 1.0f * z1;    /* maddaz.w */
        key.f[W] = acc + 1.0f * z2; /* maddz.w */
        vf[VF_KEYSUM].f[W] = vf[VF_KEYSUM].f[W] + key.f[W];
        vu1_st(VU1_TRI + t, &key, MW);
    }
}

void vu1_sort_keys_vu0(void) {
    sort_keys(0);
}

void vu1_sort_keys_vu1(void) {
    sort_keys(1);
}

/* ---------------------------------------------------------------------- */
/* The sort (cont_2C30): ascending by key, so back to front (Z is 1/z, larger
   is nearer). The list is split around the mean key into two halves
   (0x200, 0x280), each half around its own mean into two buckets (0x300,
   0x340, 0x380, 0x3C0), each bucket is insertion-sorted back into the list
   at 0x17F, in order. Every split and insertion keeps equal keys in their
   order, so the result is a stable sort.

   The comparisons are SUBA/SUB then FSAND on the sign flag, 4 pairs later:
   the flag of that subtraction. The inner insertion loop counts with
   "iaddi vi03; ibne vi03, vi12": a branch right after the integer op that
   writes its register reads the old value (PCSX2's branch VI delay; read
   with the new one, the loop would run off the end), so it scans the whole
   output. A branch after FSAND/FMAND reads the new value. */

static int less(float a, float b) {
    return a - b < 0.0f;
}

/* Split n quadwords at src around their mean (sum / n) into lo (key <
   mean) and hi; both get every quadword written at their next slot, the
   slot advances on the side it belongs to. Returns the lo count; the sums
   are kept the way the first split keeps them (add, then take back). */
static uint32_t split(uint32_t src, uint32_t n, float mean, uint32_t lo, uint32_t hi, float *lo_sum,
                      float *hi_sum, uint32_t *nhi) {
    uint32_t nlo = 0, i;

    *nhi = 0;
    for (i = 0; i < n; i++) {
        V4 q = vu1_ld(src + i);
        int neg = less(q.f[W], mean);
        vu1_st(lo + nlo, &q, MXYZW);
        vu1_st(hi + *nhi, &q, MXYZW);
        if (lo_sum != NULL) {
            float k = q.f[W] + 0.0f; /* add.w vf02, vf01, vf31 */
            *lo_sum = *lo_sum + q.f[W];
            if (!neg) {
                *lo_sum = *lo_sum - k;
                *hi_sum = *hi_sum + k;
            }
        }
        if (neg) {
            nlo++;
        } else {
            (*nhi)++;
        }
    }
    return nlo;
}

/* sub_2F98: insertion sort of n quadwords at src into out */
static void insertion_sort(uint32_t src, uint32_t n, uint32_t out) {
    uint32_t k, j, i;

    {
        V4 e0 = vu1_ld(src);
        vu1_st(out, &e0, MXYZW);
    }
    for (k = 1; k < n; k++) {
        V4 e = vu1_ld(src + k);
        for (j = 0; j < k; j++) {
            if (less(e.f[W], vu1_ld(out + j).f[W])) {
                break;
            }
        }
        for (i = k; i > j; i--) {
            V4 m = vu1_ld(out + i - 1);
            vu1_st(out + i, &m, MXYZW);
        }
        vu1_st(out + j, &e, MXYZW);
    }
}

void vu1_sort(void) {
    V4 *vf = grx_vu1.vf;
    uint32_t ntri = vu1_ntri(), nlo, nhi, n[4], out, b;
    static const uint32_t bucket[4] = {0x300, 0x340, 0x380, 0x3C0};
    float lo_sum = 0.0f, hi_sum = 0.0f, mean; /* mulx.w vf08/vf09 by vf00x */

    mean = vu_div(vf[VF_KEYSUM].f[W], vf[VF_KEYCNT].f[W]);
    nlo = split(VU1_TRI, ntri, mean, 0x200, 0x280, &lo_sum, &hi_sum, &nhi);
    n[0] = n[1] = n[2] = n[3] = 0;
    if (nlo != 0) {
        mean = vu_div(lo_sum, vu_itof((int16_t)nlo, 0));
        n[0] = split(0x200, nlo, mean, bucket[0], bucket[1], NULL, NULL, &n[1]);
    }
    if (nhi != 0) {
        mean = vu_div(hi_sum, vu_itof((int16_t)nhi, 0));
        n[2] = split(0x280, nhi, mean, bucket[2], bucket[3], NULL, NULL, &n[3]);
    }
    out = VU1_TRI;
    for (b = 0; b < 4; b++) {
        if (n[b] != 0) {
            insertion_sort(bucket[b], n[b], out);
            out += n[b];
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Assembly (cont_0DF0, cont_0F30, cont_10A0, cont_1258): the GIF tag from
   the header to 0x1BF, then per triangle 9 quadwords from 0x1C0: ST, RGBA,
   XYZF2 of each vertex. cull 1 (mode 0) marks a triangle whose screen
   cross product (b - a) x (c - a) has a negative z, cull 2 (mode 1) one
   whose z is not negative: ISW 0x8000 into the last XYZF2's w sets the ADC
   bit (and clears F), so the GS doesn't draw it. The sign comes from
   OPMSUB's MAC flag (Sz), read by FMAND 4 pairs later. */

void vu1_assemble(int vu1_indices, int cull) {
    uint32_t ntri = vu1_ntri(), t, out = VU1_PACKET, idx[3];
    int k;

    vu1_st(VU1_GIFTAG, &grx_vu1.vf[VF_GIFTAG], MXYZW);
    if (ntri == 0) {
        vu1_warn("assemble: no triangles", 0);
    }
    for (t = 0; t < ntri; t++) {
        tri_indices(t, vu1_indices, idx);
        for (k = 0; k < 3; k++) {
            V4 st = vu1_ld(VU1_NRM + idx[k]), rgba = vu1_ld(VU1_COL + idx[k]), xyzf = vu1_ld(VU1_POS + idx[k]);
            vu1_st(out++, &st, MXYZW);
            vu1_st(out++, &rgba, MXYZW);
            vu1_st(out++, &xyzf, MXYZW);
        }
        if (cull) {
            V4 a = vu1_ld(VU1_POS + idx[0]), b = vu1_ld(VU1_POS + idx[1]), c = vu1_ld(VU1_POS + idx[2]);
            float ax = vu_itof(a.i[X], 4), ay = vu_itof(a.i[Y], 4);
            float e1x = vu_itof(b.i[X], 4) - ax, e1y = vu_itof(b.i[Y], 4) - ay; /* sub.xy vf10 */
            float e2x = vu_itof(c.i[X], 4) - ax, e2y = vu_itof(c.i[Y], 4) - ay; /* sub.xy vf11 */
            float acc = e1x * e2y;         /* opmula.z */
            float z = acc - e2x * e1y;     /* opmsub.z */
            int neg = z < 0.0f;
            if (cull == 1 ? neg : !neg) {
                vu1_isw(out - 1, W, 0x8000);
            }
        }
    }
    vu1_kick(VU1_GIFTAG);
}

/* ---------------------------------------------------------------------- */
/* XGKICK: the GIF packet from qa, through its EOP tag. VU memory wraps. */

void vu1_kick(uint32_t qa) {
    static V4 copy[1024];
    uint32_t p = qa & VU1_QW_MASK, n = 0;

    for (;;) {
        V4 tag = vu1_ld(p + n);
        uint32_t nloop = tag.u[0] & 0x7FFF, eop = (tag.u[0] >> 15) & 1;
        uint32_t flg = (tag.u[1] >> 26) & 3, nreg = tag.u[1] >> 28;
        uint32_t data;
        if (nreg == 0) {
            nreg = 16;
        }
        data = flg == 0 ? nloop * nreg : flg == 1 ? (nloop * nreg + 1) / 2 : nloop;
        n += 1 + data;
        if (eop || n >= 1024) {
            break;
        }
    }
    if (n > 1024) {
        vu1_warn("xgkick: packet longer than VU memory", n);
        n = 1024;
    }
    if (p + n <= 1024) {
        grx_gs_gif(grx_vu1_data + (p << 4), n, GRX_GIF_PATH1);
    } else {
        uint32_t i;
        for (i = 0; i < n; i++) {
            copy[i] = vu1_ld(p + i);
        }
        grx_gs_gif(copy, n, GRX_GIF_PATH1);
    }
}
