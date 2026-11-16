/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
} Mdl;

typedef struct {
    u8 pad0[0x30];
    f32 x30;
    u8 pad34[0x54 - 0x34];
    s32 x54;
    u8 pad58[0x60 - 0x58];
    Mdl *mdl;
    u8 pad64[0x180 - 0x64];
    FVECTOR x180;
} Chara;

f32 VuVecDot(f32 *, f32 *);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 ChrMoveTowardPos(Chara *c, f32 *target, f32 r, f32 speed) {
    FVECTOR d;
    FVECTOR a;
    f32 len;
    f32 inv;

    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(d), "r"(target), "r"(c->mdl->pos) : "memory");
    d[1] = 0.0f;
    len = VuVecDot(d, d);
    inv = 1.0f / fsqrt(len);
    if (r < len) {
        c->x30 = r;
        c->x180[0] = d[0] * inv * r * speed;
        c->x180[1] = d[1] * inv * r * speed;
        c->x180[2] = d[2] * inv * r * speed;
    } else {
        c->x30 = len;
        c->x180[0] = d[0] * speed;
        c->x180[1] = d[1] * speed;
        c->x180[2] = d[2] * speed;
    }
    FldDirToRot(c->x180, a);
    c->x54 = (u16)(s32)(a[1] * 10430.378f);
    return 0;
}
