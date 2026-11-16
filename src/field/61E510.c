/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    s32 unk0;
    u8 pad4[0x1C];
    FVECTOR unk20;
    FVECTOR unk30;
} Hit;

typedef struct {
    u8 pad[0x50];
} Elem;

typedef struct {
    s32 count;
    Elem *elems;
} List;

s32 FldCollSphereTriTest(Elem *, Hit *);
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

s32 FldCollTrisSweep(Hit *h)
{
    FVECTOR best;
    f32 min;
    f32 d;
    s32 found;
    s32 i;
    s32 n;
    List *l;
    Elem *elems;

    l = MapPolyGetList();
    n = l->count;
    if (n == 0) {
        return 0;
    }
    elems = l->elems;
    found = 0;
    min = fsqrt(VuVecDot(h->unk20, h->unk20));
    copyq(best, h->unk30);
    for (i = 0; i < n; i++) {
        h->unk0 = 0;
        FldCollSphereTriTest(&elems[i], h);
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
