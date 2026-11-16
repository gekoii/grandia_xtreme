#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

void VuSinCosAngle(f32 *, f32 *, s32);

void CamCalcEyePos(f32 *base, f32 *out, f32 ry, f32 rx, f32 dist)
{
    FVECTOR dir;
    FVECTOR tmp;
    f32 v[4];
    f32 k = 10430.378f;

    VuSinCosAngle(&v[0], &v[1], (s32)(ry * k));
    VuSinCosAngle(&v[2], &v[3], (s32)(rx * k));
    dir[0] = -v[0] * v[3];
    dir[1] = -v[2];
    dir[2] = v[1] * v[3];
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "qmtc2.ni %2, $vf2\n\t"
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(tmp), "r"(dir), "r"(-dist) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(out), "r"(tmp), "r"(base) : "memory");
}
