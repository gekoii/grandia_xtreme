/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR unk0;
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
    u8 pad40[0x10];
} Item;

typedef struct {
    s32 unk0;
    u8 pad4[0x8];
    f32 unkC;
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
    FVECTOR unk40;
} Obj;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

#define VSUB(out, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(b) : "memory")

#define VADD(out, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(b) : "memory")

#define VSCALE(out, a, s) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "qmtc2.ni %2, $vf2\n\t" \
    "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(s) : "memory")

#define VCROSS(out, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vopmula.xyz ACC, $vf1, $vf2\n\t" \
    "vopmsub.xyz $vf1, $vf2, $vf1\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(b) : "memory")

s32 MapPolySweepHit(Item *p, Obj *o)
{
    FVECTOR n;
    FVECTOR d;
    FVECTOR v;
    FVECTOR s;
    FVECTOR h;
    FVECTOR w;
    FVECTOR e;
    FVECTOR q;
    FVECTOR e1;
    FVECTOR e2;
    f32 r;
    f32 dist;
    f32 dv;
    f32 k;
    f32 x;
    f32 zero;

    r = o->unkC;
    VuVecCopy(n, p->unk30);
    VuVecCopy(e1, p->unk10);
    VuVecCopy(e2, p->unk20);
    VuVecCopy(v, o->unk20);
    VSUB(d, o->unk10, p->unk0);
    dist = VuVecDot(n, d);
    zero = 0.0f;
    if (dist < zero) {
        return 0;
    }
    dv = -VuVecDot(n, v);
    if (dv < zero) {
        return 0;
    }
    if (__builtin_fabsf(dv) < 0.001f) {
        return 0;
    }
    k = dist - r;
    if (dv < k) {
        return 0;
    }
    k = k / dv;
    if (k < zero) {
        return 0;
    }
    VSCALE(s, v, k);
    VADD(h, s, d);
    VuVecCopy(q, s);
    VuVecCopy(w, s);
    VuVecCopy(e, s);
    VCROSS(n, p->unk30, p->unk10);
    VuVecNormalize(n, n);
    VuVecCopy(s, h);
    x = VuVecDot(n, s);
    if (zero < x && r < x) {
        return 0;
    }
    VCROSS(n, p->unk20, p->unk30);
    VuVecNormalize(n, n);
    VuVecCopy(s, h);
    x = VuVecDot(n, s);
    if (0.0f < x && r < x) {
        return 0;
    }
    VuVecCopy(w, p->unk30);
    VSUB(e, p->unk20, p->unk10);
    VCROSS(n, w, e);
    VuVecNormalize(n, n);
    VSUB(s, h, p->unk10);
    x = VuVecDot(n, s);
    if (0.0f < x && r < x) {
        return 0;
    }
    return 1;
}
