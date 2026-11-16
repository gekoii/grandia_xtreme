#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR pos;
    FVECTOR dir;
    f32 x20;
    f32 x24;
} Obj6533E0;

f32 VuVecNormalizeLen(f32 *, f32 *);

#define vscale(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

void MiniBallAddVelocity(Obj6533E0 *o, f32 dx, f32 dz)
{
    f32 d;

    vscale(o->dir, o->dir, o->x24);
    o->dir[0] += dx;
    o->dir[2] += dz;
    d = VuVecNormalizeLen(o->dir, o->dir);
    o->x24 = d;
    o->x20 = d;
}
