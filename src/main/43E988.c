/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

void VuVecNormalize(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

#define VSUBW(o, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyzw $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(b) : "memory")

#define VSUB(o, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(b) : "memory")

#define VCROSS(o, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vopmula.xyz ACC, $vf1, $vf2\n\t" \
    "vopmsub.xyz $vf1, $vf2, $vf1\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(b) : "memory")

s32 VuPointInTriangle(f32 *pos, FVECTOR *tri, f32 eps)
{
    f32 *p0 = (f32 *)0x70003000;
    f32 *d = (f32 *)0x70003010;
    f32 *p1 = (f32 *)0x70003020;
    f32 *p2 = (f32 *)0x70003030;
    f32 *e = (f32 *)0x70003040;
    f32 *n = (f32 *)0x70003050;
    f32 *p3 = (f32 *)0x70003060;

    copyq(p0, tri[0]);
    VSUBW(d, pos, p0);
    copyq(p1, tri[1]);
    copyq(p2, tri[2]);
    copyq(p3, tri[3]);
    VCROSS(n, p3, p1);
    VuVecNormalize((f32 *)0x70003050, (f32 *)0x70003050);
    if (eps < VuVecDot((f32 *)0x70003050, (f32 *)0x70003010)) {
        return 0;
    }
    VCROSS(n, p2, p3);
    VuVecNormalize((f32 *)0x70003050, (f32 *)0x70003050);
    if (eps < VuVecDot((f32 *)0x70003050, (f32 *)0x70003010)) {
        return 0;
    }
    VSUB(e, p2, p1);
    VSUB(d, d, p1);
    VCROSS(n, p3, e);
    VuVecNormalize((f32 *)0x70003050, (f32 *)0x70003050);
    if (eps < VuVecDot((f32 *)0x70003050, (f32 *)0x70003010)) {
        return 0;
    }
    return 1;
}
