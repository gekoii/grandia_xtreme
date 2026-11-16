#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

void VuVecNormalize(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

void VuVecProject(f32 *out, f32 *v, f32 *dir)
{
    FVECTOR n;
    f32 d;

    VuVecNormalize(n, dir);
    d = VuVecDot(n, v);
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "qmtc2.ni %2, $vf2\n\t"
        "vmulx.xyzw $vf1, $vf1, $vf2x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(out), "r"(n), "r"(d) : "memory");
}
