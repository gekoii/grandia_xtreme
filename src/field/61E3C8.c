/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

typedef struct {
    s32 unk0;
    u8 pad4[0x1C];
    FVECTOR unk20;
    FVECTOR unk30;
} Hit;

void FldCollShapeTest(void *, Hit *);
void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

#define VSCALE(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

s32 FldCollShapesSweep(Hit *h)
{
    FVECTOR best;
    f32 min;
    f32 d;
    s32 found;
    s32 i;
    s32 n;

    CamDebugTriClear();
    min = fsqrt(VuVecDot(h->unk20, h->unk20));
    copyq(best, h->unk30);
    found = 0;
    n = mapHtaHitList.count;
    for (i = 0; i < n; i++) {
        FldCollShapeTest(mapHtaHitList.areas[i], h);
        if (h->unk0) {
            d = fsqrt(VuVecDot(h->unk30, h->unk30));
            if (d < min) {
                min = d;
                found = 1;
                VuVecCopy(best, h->unk30);
                VuVecCopy(h->unk20, h->unk30);
                h->unk0 = found;
            }
        }
    }
    if (found) {
        VSCALE(h->unk20, best, 0.998f);
        VSCALE(h->unk30, best, 0.998f);
        h->unk0 = 1;
    }
    return 0;
}
