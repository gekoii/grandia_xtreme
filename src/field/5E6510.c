/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR v[4];
    u8 pad40[0x10];
} Item;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

#define VADD(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VSUB(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VCROSS(o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vopmula.xyz ACC, $vf1, $vf2\n\t" \
        "vopmsub.xyz $vf1, $vf2, $vf1\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define E0 ((f32 *)0x70003000)
#define E1 ((f32 *)0x70003010)
#define E2 ((f32 *)0x70003020)
#define NRM ((f32 *)0x70003030)
#define PT ((f32 *)0x70003040)
#define TMP ((f32 *)0x70003050)

s32 MapPolyHitTest(f32 *pos, f32 *ofs, Item *it, f32 r)
{
    f32 *nrm = NRM;
    f32 *e0 = E0;
    f32 *e1 = E1;
    f32 *e2 = E2;
    f32 *pt = PT;
    f32 *tmp = TMP;
    f32 d;

    copyq(pt, pos);
    VADD(pt, pt, ofs);
    VSUB(pt, pt, it->v[0]);
    VuVecCopy(E0, it->v[1]);
    VuVecCopy(E1, it->v[2]);
    VuVecCopy(NRM, it->v[3]);
    if (__builtin_fabsf(nrm[1]) < 0.2f) {
        d = VuVecDot(NRM, PT);
        if (d < 0.0f) {
            return 0;
        }
        if (r < d) {
            return 0;
        }
        VCROSS(tmp, nrm, e0);
        VuVecNormalize(TMP, TMP);
        if (r < VuVecDot(PT, TMP)) {
            return 0;
        }
        VCROSS(tmp, e1, nrm);
        VuVecNormalize(TMP, TMP);
        if (r < VuVecDot(PT, TMP)) {
            return 0;
        }
        VSUB(e2, e1, e0);
        VSUB(pt, pt, e0);
        VCROSS(tmp, nrm, e2);
        VuVecNormalize(TMP, TMP);
        if (!(r < VuVecDot(PT, TMP))) {
            return 1;
        }
        return 0;
    }
    if (nrm[1] > 0.0f) {
        NRM[1] = 1.0f;
        NRM[2] = 0.0f;
        NRM[0] = 0.0f;
    } else {
        NRM[1] = -1.0f;
        NRM[2] = 0.0f;
        NRM[0] = 0.0f;
    }
    ((s32 *)PT)[1] = 0;
    ((s32 *)E0)[1] = 0;
    VCROSS(tmp, nrm, e0);
    VuVecNormalize(TMP, TMP);
    if (r < VuVecDot(TMP, PT)) {
        return 0;
    }
    ((s32 *)E1)[1] = 0;
    VCROSS(tmp, e1, nrm);
    VuVecNormalize(TMP, TMP);
    if (r < VuVecDot(TMP, PT)) {
        return 0;
    }
    VSUB(e2, e1, e0);
    VSUB(pt, pt, e0);
    VCROSS(tmp, nrm, e2);
    VuVecNormalize(TMP, TMP);
    if (!(r < VuVecDot(TMP, PT))) {
        return 1;
    }
    return 0;
}
