/* Port-only C for VuSinCosAngle (0x0045D7A8), which the decompilation still has in assembly. */
#include "common.h"

void VuSinCosAngle(f32 *sinOut, f32 *cosOut, s32 angle);

static f32 FromBits(u32 bits) {
    union {
        u32 u;
        f32 f;
    } v;

    v.u = bits;
    return v.f;
}

/* A 16-bit angle folded into [-0x4000, 0x4000] (the sine's argument) */
static s32 FoldAngle(u32 a) {
    a &= 0xFFFF;
    if ((s32)a > 0x8000) {
        a = 0x10000 - a;
    }
    return (s32)a - 0x4000;
}

/* The odd polynomial to x^9, each power a lane of a VU0 register, in the
   asm's order of products and sums */
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

/* sin and cos (the sine of angle + 0x4000) of a 16-bit angle, both at once
   on VU0 (vf1-vf7 are left changed there; no caller reads them) */
void VuSinCosAngle(f32 *sinOut, f32 *cosOut, s32 angle) {
    f32 k = FromBits(0x38C90FDB);
    f32 s = (f32)FoldAngle(angle + 0x4000) * k;
    f32 c = (f32)FoldAngle(angle + 0x8000) * k;

    *sinOut = SinPoly(s);
    *cosOut = SinPoly(c);
}
