/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n17.h"

typedef struct {
    s32 unk0;
    u8 pad4[0x1C];
    FVECTOR unk20;
    FVECTOR unk30;
    FVECTOR unk40;
} Obj;

f32 VuVecDot(f32 *, f32 *);
s32 MapPolySweepHit(MapPolyTri *, Obj *);
s32 MapPolySweepSlide(MapPolyTri *, Obj *);

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

s32 MapPolySweep(Obj *o)
{
    FVECTOR save[2];
    MapPolyList *t = &mapPolyList;
    s32 n = t->count;
    MapPolyTri *items;
    s32 found;
    s32 i;
    f32 len;

    if (n == 0) {
        return 0;
    }
    items = t->tris;
    found = 0;
    len = fsqrt(VuVecDot(o->unk20, o->unk20));
    for (i = 0; i < n; i++) {
        if (MapPolySweepHit(&items[i], o) && MapPolySweepSlide(&items[i], o)) {
            if (found) {
                if (fsqrt(VuVecDot(o->unk30, o->unk30)) < len) {
                    copyq(o->unk30, save[0]);
                    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(o->unk40) : "memory");
                    return 1;
                }
            } else {
                copyq(save[0], o->unk30);
                copyq(save[1], o->unk40);
                len = fsqrt(VuVecDot(save[0], save[0]));
                found = 1;
            }
            o->unk0 = 1;
        }
    }
    return found;
}
