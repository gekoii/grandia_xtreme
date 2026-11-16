/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "gph.h"
#include "protos/battle.h"


typedef struct {
    u8 pad0[0x84];
    f32 unk84;
} Unit;

typedef struct {
    u8 unk0;
    u8 pad1[0x27];
    Unit *unk28;
} Obj;

extern FVECTOR fxHitColors[];

void VuVecCopy(void *, void *);

#define VSCALE(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

#define VSUB(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

void FxHitSlotDraw(Obj *o)
{
    Unit *u;
    f32 t;
    f32 k;
    f32 *v;
    f32 *p;

    u = o->unk28;
    if (u == NULL) {
        return;
    }
    switch (o->unk0 & 0xF0) {
    case 0x30:
    case 0x40:
    case 0x50:
        break;
    default:
        t = u->unk84;
        v = fxHitColors[o->unk0 & 0xF];
        if (t < 8.0f) {
            k = 1.0f;
        } else if (t < 10.0f) {
            k = (8.0f - t) * 0.050000012f + 1.0f;
        } else if (t < 12.0f) {
            k = (10.0f - t) * 0.054999977f + 0.9f;
        } else if (t < 14.0f) {
            k = (12.0f - t) * 0.095f + 0.79f;
        } else {
            k = (14.0f - t) * 0.1f + 0.6f;
        }
        p = BtlChrFindPart(o->unk28, 1)->color;
        VSCALE(p, v, k);
        VSUB(p, p, fxHitColors);
        if (t < 2.0f) {
            k = (0.0f - t) * -0.5f + 0.0f;
        } else if (t < 4.0f) {
            k = 1.0f;
        } else {
            k = (4.0f - t) * 0.11111111f + 1.0f;
        }
        p = BtlChrFindPart(o->unk28, 2)->color;
        VSCALE(p, v, k);
        VSUB(p, p, fxHitColors);
        VuVecCopy(BtlChrFindPart(o->unk28, 3)->color, p);
        if (t < 3.0f) {
            k = 1.0f;
        } else {
            k = (3.0f - t) * 0.2f + 1.0f;
        }
        p = BtlChrFindPart(o->unk28, 4)->color;
        VSCALE(p, v, k);
        VSUB(p, p, fxHitColors);
        break;
    }
    BtlChrModelCalcDraw(o->unk28);
}
