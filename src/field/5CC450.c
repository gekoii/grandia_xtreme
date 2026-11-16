#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

f32 VuVecDot(void *, void *);

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

f32 CamTrailLength(void)
{
    FVECTOR d;
    FVECTOR a;
    FVECTOR b;
    s32 i;
    s32 n;
    f32 sum;

    i = 0;
    sum = 0.0f;
    n = CamTrailCount() + 1;
    for (; i < n;) {
        CamTrailGet(i, a);
        i++;
        CamTrailGet(i, b);
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vsub.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(d), "r"(a), "r"(b) : "memory");
        sum += fsqrt(VuVecDot(d, d));
    }
    return sum;
}
