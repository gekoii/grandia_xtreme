#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));
typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));

typedef struct {
    u8 pad[0x10];
    FVECTOR pos;
} Node;

typedef struct {
    u8 pad0[0x7];
    u8 flags;
    u8 pad8[0x4];
    Node *node;
    u8 pad10[0x10];
    FVECTOR pos;
    u8 pad30[0x10];
    FVECTOR ofs;
    FVECTOR scale;
    u8 pad60[0x20];
    FMATRIX mtx;
    FVECTOR unkC0;
    FVECTOR unkD0;
    f32 unkE0;
} Obj;

typedef struct {
    u8 pad[0x50];
} Elem;

extern Elem fldCollShapeTris[];
void VuVecCopy(f32 *, f32 *);
void FldCollTriBuild(Elem *, f32 *, f32 *, f32 *);
s32 FldCollSphereTriTest(Elem *, s32);

#define VADD(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

#define VMUL(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vmul.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

#define VAPPLY(dst, m, v) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%2)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(m), "r"(v) : "memory")

void FldCollShapeWallTest(Obj *o, s32 arg1)
{
    FVECTOR base;
    FVECTOR v[4];
    f32 k;
    s32 i;
    Elem *p;

    if (!(o->flags & 4)) {
        if (o->node != NULL) {
            VADD(base, o->pos, o->node->pos);
        } else {
            VuVecCopy(base, o->pos);
        }
    }
    VuVecCopy(v[0], o->unkC0);
    VuVecCopy(v[1], o->unkD0);
    VuVecCopy(v[2], o->unkC0);
    VuVecCopy(v[3], o->unkD0);
    k = o->scale[1] * o->unkE0;
    v[2][1] += k;
    v[3][1] += k;
    if (o->flags & 4) {
        for (i = 0; i < 4; i++) {
            VMUL(v[i], o->scale, v[i]);
            VADD(v[i], v[i], o->ofs);
            VAPPLY(v[i], o->mtx, v[i]);
        }
    } else {
        for (i = 0; i < 4; i++) {
            VMUL(v[i], v[i], o->scale);
            VADD(v[i], v[i], base);
        }
    }
    p = fldCollShapeTris;
    FldCollTriBuild(p, v[0], v[1], v[2]);
    p++;
    FldCollTriBuild(p, v[1], v[3], v[2]);
    p++;
    FldCollTriBuild(p, v[0], v[2], v[1]);
    p++;
    FldCollTriBuild(p, v[1], v[2], v[3]);
    p = fldCollShapeTris;
    for (i = 0; i < 4; i++) {
        FldCollSphereTriTest(p, arg1);
        p++;
    }
}
