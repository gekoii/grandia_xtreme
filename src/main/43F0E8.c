/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR pos;
    FVECTOR normal;
    f32 f20;
    f32 f24;
} Plane;

f32 VuVecDot(f32 *, f32 *);

#define VSUB(o, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(b) : "memory")

#define VSCALE(o, a, s) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "qmtc2.ni %2, $vf2\n\t" \
    "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(s) : "memory")

s32 VuCapsuleSphereHit(Plane *p, f32 *s)
{
    FVECTOR d;
    FVECTOR t;
    FVECTOR e;
    f32 r;
    f32 dist;

    r = p->f24 + s[3];
    VSUB(d, s, p->pos);
    dist = VuVecDot(d, p->normal);
    VSCALE(t, p->normal, dist);
    VSUB(e, d, t);
    if (VuVecDot(e, e) > r * r) {
        return 0;
    }
    if (p->f20 + r < dist) {
        return 0;
    }
    if (dist < -r) {
        return 0;
    }
    return 1;
}
