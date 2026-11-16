#include "common.h"
#include "protos/main.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[8];
    u16 flags;
    u8 padA[2];
    f32 *mtx;
} Part;

typedef struct {
    u8 pad0[8];
    u16 flags;
    u8 padA[0x20 - 0xA];
    FVECTOR pos;
    u8 pad30[0x70 - 0x30];
    s32 unk70;
} Obj;

void VuMtxRotate(f32 *, f32 *);

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void GphModelCalcCull(Obj *o, f32 scale)
{
    FVECTOR v;
    FVECTOR saved;
    s16 flags;
    s16 s70;
    Part *p;

    if (o->flags & 0x200) {
        flags = o->flags;
        s70 = o->unk70;
        copyq(saved, o->pos);
        o->unk70 = 3;
        p = ChrModelGetPart(o, 1);
        p->flags |= 0x100;
        __asm__ volatile(
            "vmr32.xyzw $vf5, $vf0\n\t"
            "vmr32.xyzw $vf6, $vf5\n\t"
            "vmr32.xyzw $vf7, $vf6\n\t"
            "sqc2 $vf0, 0x30(%0)\n\t"
            "sqc2 $vf5, 0x20(%0)\n\t"
            "sqc2 $vf6, 0x10(%0)\n\t"
            "sqc2 $vf7, 0x0(%0)"
            : : "r"(p->mtx) : "memory");
        copyq(v, saved);
        v[1] = 0.0f;
        VuMtxRotate(p->mtx, v);
        o->pos[0] = 0.0f;
        o->pos[2] = 0.0f;
        GphModelCalcMatrices(o, scale);
        copyq(o->pos, saved);
        o->flags = flags;
        o->unk70 = s70;
        p->flags &= ~0x100;
    } else {
        GphModelCalcMatrices(o, scale);
    }
}
