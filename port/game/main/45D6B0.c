/* Port-only C for VuSinAngle (0x0045D6B0), which the decompilation still has in assembly. */
#include "common.h"

f32 VuSinAngle(s32 angle);

static f32 FromBits(u32 bits) {
    union {
        u32 u;
        f32 f;
    } v;

    v.u = bits;
    return v.f;
}

/* sin of a 16-bit angle (0x10000 = a full turn): folded into
   [-0x4000, 0x4000], scaled by 2pi/0x10000 and put through an odd
   polynomial to x^9 on VU0 (vf1-vf10 and ACC are left changed there; no
   caller reads them). The sum's order is the asm's. */
f32 VuSinAngle(s32 angle) {
    u32 a = (angle + 0x4000) & 0xFFFF;
    f32 x, x2, x3, x4, x5, x7, x9, acc;

    if ((s32)a > 0x8000) {
        a = 0x10000 - a;
    }
    x = (f32)(s32)(a - 0x4000) * FromBits(0x38C90FDB);
    x2 = x * x;
    x4 = x2 * x2;
    x3 = x * x2;
    x5 = x * x4;
    x7 = x3 * x4;
    x9 = x5 * x4;
    acc = x9 * FromBits(0x362E9C14);
    acc = acc + x7 * FromBits(0xB94FB21F);
    acc = acc + x5 * FromBits(0x3C08873E);
    acc = acc + x3 * FromBits(0xBE2AAAA4);
    return acc + x * 1.0f;
}
