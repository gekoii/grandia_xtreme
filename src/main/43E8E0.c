/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR p0;
    FVECTOR p1;
    FVECTOR p2;
    FVECTOR normal;
} Tri;

void VuVecNormalize(f32 *, f32 *);
void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

s32 VuTriPlaneNear(f32 *pos, Tri *t, f32 eps)
{
    FVECTOR d;
    FVECTOR n;

    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(d), "r"(pos), "r"(t->p0) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vopmula.xyz ACC, $vf1, $vf2\n\t"
        "vopmsub.xyz $vf1, $vf2, $vf1\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(n), "r"(t->p2), "r"(t->p1) : "memory");
    VuVecNormalize(n, n);
    VuVecCopy(t->normal, n);
    t->normal[3] = 0.0f;
    if (eps < fabsf(VuVecDot(d, n))) {
        return 0;
    }
    return 1;
}
