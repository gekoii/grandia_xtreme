/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[9];
    void *unkC;
    FVECTOR pos;
    f32 unk20;
    u8 pad24[0x80 - 0x24];
} Elem;

typedef struct {
    u8 pad0[0x58];
    Elem *unk58;
    u16 unk5C;
} Obj;

typedef struct {
    FVECTOR pos;
    f32 unk10;
} Tgt;

f32 VuVecDot(f32 *, f32 *);
void GphGaObjDeformNearPoint(Elem *, Tgt *);

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

void GphModelDeformNearPoint(Obj *o, Tgt *t)
{
    FVECTOR v;
    s32 n;
    f32 r;
    Elem *e;

    n = o->unk5C;
    r = t->unk10;
    e = o->unk58;
    for (; n > 0; n--, e++) {
        if (e->unkC == NULL || (e->unk2 & 0x80)) {
            continue;
        }
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vsub.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(v), "r"(e->pos), "r"(t->pos) : "memory");
        v[1] = 0.0f;
        if (fsqrt(VuVecDot(v, v)) < (r + e->unk20) * 12.0f) {
            GphGaObjDeformNearPoint(e, t);
        }
    }
}
