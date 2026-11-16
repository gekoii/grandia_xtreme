#include "common.h"
#include "protos/field.h"


s32 FldCalcYaw(f32 *a, f32 *b) {
    f32 d[4];
    f32 r[4];

    __asm__ volatile(
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf2, 0(%1)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%2)"
        : : "r"(a), "r"(b), "r"(d) : "memory");
    FldDirToRot(d, r);
    return r[1] * 57.295776f * 182.04445f;
}
