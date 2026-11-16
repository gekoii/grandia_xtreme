#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    FVECTOR a[8];
    FVECTOR b[8];
    FVECTOR c[4];
    FVECTOR d[4];
} Obj;

#define VZERO(v) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%0)\n\t" \
        "vmulx.xyzw $vf1, $vf0, $vf0x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(v) : "memory")

void GphLightVuBlockReset(Obj *p)
{
    s32 i;
    f32 *v;

    p->unk0 = 0;
    p->unk4 = 0;
    p->unkC = 0x6C100180;
    p->unk8 = 0;
    v = p->a[0];
    for (i = 0; i < 8; i++) {
        VZERO(v);
        v += 4;
    }
    for (i = 0; i < 8; i++) {
        VZERO(v);
        v += 4;
    }
    for (i = 0; i < 4; i++) {
        v[0] = 0.0f;
        v[1] = -1.0f;
        v[2] = 0.0f;
        v[3] = 0.0f;
        v += 4;
    }
    for (i = 0; i < 4; i++) {
        v[0] = 0.0f;
        v[1] = 1000.0f;
        v[2] = 0.0f;
        v[3] = 1.0f;
        v += 4;
    }
}
