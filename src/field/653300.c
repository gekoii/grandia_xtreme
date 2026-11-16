/* as: ee-gcc2.9-991111-01 */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

f32 VuVecDot(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);

#define vscale(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

#define vsub(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

f32 MiniBallSweepCircle(f32 *p, f32 *dir, f32 r)
{
    FVECTOR t;
    FVECTOR u;
    FVECTOR w;
    f32 d;
    f32 k;
    f32 e;

    d = VuVecNormalizeLen(w, p);
    if (d <= r) {
        return d - r;
    }
    k = VuVecDot(p, dir);
    vscale(t, dir, k);
    vsub(u, p, t);
    e = VuVecNormalizeLen(w, u);
    if (r < e) {
        return 10000.0f;
    }
    return k - fsqrt(r * r - e * e);
}
