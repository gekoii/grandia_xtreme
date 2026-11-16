#include "common.h"

void PtclBtlDustPuffUpdate(f32 *p)
{
    p[0] -= 0.016666668f;
    p[1] += 0.8f;
    p[5] += 0.5f;
    __asm__ volatile(
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf2, 0(%1)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(p + 4), "r"(p + 8) : "memory");
}
