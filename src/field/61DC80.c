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
extern FVECTOR fldCollUnitBoxCorners[];
extern FVECTOR fldCollBoxCorners[];
void VuVecCopy(f32 *, f32 *);
void FldCollTriBuild(Elem *, f32 *, f32 *, f32 *);
s32 FldCollSphereTriTest(Elem *, s32);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

#define VSCALE(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

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

void FldCollShapeBoxTest(Obj *o, s32 arg1)
{
    FVECTOR t;
    FVECTOR sc;
    FVECTOR base;
    FVECTOR *v;
    Elem *p;
    s32 i;

    VSCALE(t, o->unkC0, 1.0f);
    VMUL(sc, t, o->scale);
    if (!(o->flags & 4)) {
        if (o->node != NULL) {
            VADD(base, o->pos, o->node->pos);
        } else {
            VuVecCopy(base, o->pos);
        }
    }
    for (i = 0; i < 8; i++) {
        copyq(&fldCollBoxCorners[i], &fldCollUnitBoxCorners[i]);
    }
    if (o->flags & 4) {
        for (i = 0; i < 8; i++) {
            VMUL(fldCollBoxCorners[i], sc, fldCollBoxCorners[i]);
            VADD(fldCollBoxCorners[i], fldCollBoxCorners[i], o->ofs);
            VAPPLY(fldCollBoxCorners[i], o->mtx, fldCollBoxCorners[i]);
        }
    } else {
        for (i = 0; i < 8; i++) {
            VuVecCopy(fldCollBoxCorners[i], fldCollUnitBoxCorners[i]);
            VMUL(fldCollBoxCorners[i], fldCollBoxCorners[i], sc);
            VADD(fldCollBoxCorners[i], fldCollBoxCorners[i], base);
        }
    }
    v = fldCollBoxCorners;
    p = fldCollShapeTris;
    FldCollTriBuild(p, v[0], v[1], v[2]);
    p++;
    FldCollTriBuild(p, v[2], v[3], v[0]);
    p++;
    FldCollTriBuild(p, v[0], v[4], v[5]);
    p++;
    FldCollTriBuild(p, v[5], v[1], v[0]);
    p++;
    FldCollTriBuild(p, v[1], v[5], v[6]);
    p++;
    FldCollTriBuild(p, v[6], v[2], v[1]);
    p++;
    FldCollTriBuild(p, v[2], v[6], v[7]);
    p++;
    FldCollTriBuild(p, v[7], v[3], v[2]);
    p++;
    FldCollTriBuild(p, v[3], v[7], v[4]);
    p++;
    FldCollTriBuild(p, v[4], v[0], v[3]);
    p++;
    FldCollTriBuild(p, v[4], v[6], v[5]);
    p++;
    FldCollTriBuild(p, v[6], v[7], v[4]);
    p = fldCollShapeTris;
    for (i = 0; i < 12; i++) {
        FldCollSphereTriTest(p, arg1);
        p++;
    }
}
