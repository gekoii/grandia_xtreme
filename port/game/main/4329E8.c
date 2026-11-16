/* Port-only C for VuMtxRotate (0x004329E8), which the decompilation still has in assembly. */
#include "common.h"

void VuMtxRotate(f32 *mtx, void *angles);

static f32 FromBits(u32 bits) {
    union {
        u32 u;
        f32 f;
    } v;

    v.u = bits;
    return v.f;
}

/* As VuSinCosAngle: the odd polynomial to x^9 in the asm's order */
static f32 SinPoly(f32 x) {
    f32 x2 = x * x;
    f32 px, py, pz, pw;

    pw = FromBits(0x362E9C14) * x2;
    pz = FromBits(0xB94FB21F) * x2;
    pw = pw * x2;
    py = FromBits(0x3C08873E) * x2;
    pz = pz * x2;
    pw = pw * x2;
    px = FromBits(0xBE2AAAA4) * x2;
    py = py * x2;
    pz = pz * x2;
    pw = pw * x2;
    px = px * x;
    py = py * x;
    pz = pz * x;
    pw = pw * x;
    px = px + x;
    pz = pz + pw;
    py = py + pz;
    return px + py;
}

/* The sine and cosine arguments of one angle, folded into [-0x4000, 0x4000]
   with MMI halfword ops: the sine's in the word's low half, the cosine's in
   the high half. Halfwords are folded by adding -2h, but that add (paddw)
   and the final - 0x40004000 (psubw) are word-wide, so a carry or borrow
   from the low half moves the cosine's argument by one; kept as is. */
static void FoldAngle(u32 word, s32 *s, s32 *c) {
    u32 lo = (word + 0x4000) & 0xFFFF;
    u32 hi = (word + 0x8000) & 0xFFFF;
    u32 mlo = (lo & 0x8000) ? (0u - 2 * lo) & 0xFFFF : 0;
    u32 mhi = (hi & 0x8000) ? (0u - 2 * hi) & 0xFFFF : 0;
    u32 w = ((hi << 16) | lo) + ((mhi << 16) | mlo) - 0x40004000;

    *s = (s16)(w & 0xFFFF);
    *c = (s16)(w >> 16);
}

/* Multiplies rows 0-2 of mtx (on the left) by the rotation for the 16-bit
   angles in the low halves of angles' x, y and z words (w's is computed and
   unused); row 3 is left alone. VU0's vf1-vf21 and ACC are left changed
   there; no caller reads them. */
void VuMtxRotate(f32 *mtx, void *angles) {
    u32 *in = angles;
    f32 k = FromBits(0x38C90FDB);
    f32 sx, cx, sy, cy, sz, cz;
    f32 r[3][3];
    f32 t, u, acc;
    f32 m[3][4];
    s32 si, ci;
    s32 i, j;

    FoldAngle(in[0], &si, &ci);
    sx = SinPoly((f32)si * k);
    cx = SinPoly((f32)ci * k);
    FoldAngle(in[1], &si, &ci);
    sy = SinPoly((f32)si * k);
    cy = SinPoly((f32)ci * k);
    FoldAngle(in[2], &si, &ci);
    sz = SinPoly((f32)si * k);
    cz = SinPoly((f32)ci * k);
    /* vf1 = (sx, cx, sy, cy), vf2 = (sz, cz) */
    sx = 0.0f + sx;
    cx = 0.0f + cx;
    sy = 0.0f + sy;
    sz = 0.0f + sz;
    cz = 0.0f + cz;

    t = 0.0f + cy;
    r[0][0] = t * cz;
    r[0][1] = t * sz;
    r[0][2] = 0.0f + sy;

    t = 0.0f - sx * sy;
    acc = 0.0f - sz * cx;
    r[1][0] = acc + t * cz;
    acc = cx * cz;
    r[1][1] = acc + t * sz;
    r[1][2] = 0.0f + sx * cy;

    u = 0.0f - cx * sy;
    acc = sx * sz;
    r[2][0] = acc + u * cz;
    acc = 0.0f - cz * sx;
    r[2][1] = acc + u * sz;
    r[2][2] = 0.0f + cx * cy;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            acc = mtx[0 * 4 + j] * r[i][0];
            acc = acc + mtx[1 * 4 + j] * r[i][1];
            m[i][j] = acc + mtx[2 * 4 + j] * r[i][2];
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            mtx[i * 4 + j] = m[i][j];
        }
    }
}
