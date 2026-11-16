#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));
typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR unk10;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} Model;

typedef struct {
    u8 pad0[0x50];
    s32 unk50;
    s32 unk54;
    u8 pad58[8];
    Model *unk60;
} Obj;

s32 ChrModelGetPartXform(FVECTOR *, Model *, s32, f32);
void VuQuatToEuler(s32 *, FVECTOR *);

void ChrMotApplyRootMove(Obj *arg0) {
    FVECTOR v[3];
    FMATRIX m;
    s32 r[4] __attribute__((aligned(16)));

    if (ChrModelGetPartXform(v, arg0->unk60, 0, (f32)ChrMotGetLastFrame(arg0->unk60)) == 1) {
        VuQuatToEuler(r, &v[1]);
        arg0->unk50 = arg0->unk54 = arg0->unk54 + r[1];
    }
    r[0] = arg0->unk60->unk20;
    r[1] = arg0->unk60->unk24;
    r[2] = arg0->unk60->unk28;
    VuMtxSetRotZYX(m, r);
    __asm__ volatile(
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf4, 0x0(%1)\n\t"
        "lqc2 $vf5, 0x10(%1)\n\t"
        "lqc2 $vf6, 0x20(%1)\n\t"
        "lqc2 $vf7, 0x30(%1)\n\t"
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(v), "r"(m) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf2, 0(%1)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(arg0->unk60->unk10), "r"(v) : "memory");
}
