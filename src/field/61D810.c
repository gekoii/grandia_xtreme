/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR center;
    f32 unk20;
    u8 pad24[0x20];
    s32 count;
    u8 pad48[0x8];
    s16 *idx;
    u8 pad54[0xC];
    u8 *verts;
    u8 pad64[0x1C];
} Part;

typedef struct {
    u8 pad0[0xC];
    FVECTOR *mtx;
    u8 pad10[0x30];
    Part *parts;
    u8 pad44[0x1A];
    u16 nparts;
} Model;

extern s32 fldCollMeshHitCount;
extern s32 fldCollTriTestTotal;

extern s32 fldCollTriTestCount;
extern s32 fldCollTriBoundCount;
extern s32 fldCollTriPlaneCount;
extern s32 fldCollTriHitCount;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
/* Declared radius-second: the argument-load order only matches this way. */
s32 VuTriPlaneNear(f32 *, f32, f32 *);
s32 VuPointInTriangle(f32 *, f32, f32 *);
s32 VuSphereTriBoundHit(f32 *, f32, f32 *);

#define SPR(n) ((f32 *)(0x70003100 + (n) * 16))

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

s32 FldCollPartGatherSphere(Model *arg0, f32 *arg1, f32 arg2) {
    s32 cnt;
    FVECTOR *m;
    Part *p;
    s32 i;
    s32 j;
    s16 *idx;
    u8 *verts;
    f32 r;
    f32 d;
    s32 ok;
    f32 *t0;
    f32 *t4;

    t0 = SPR(0);
    t4 = SPR(4);
    m = arg0->mtx;
    p = arg0->parts;
    cnt = 0;
    VuVecCopy(SPR(2), arg1);
    for (i = 0; i < arg0->nparts; p++, i++) {
        VuVecCopy(SPR(0), p->center);
        t0[3] = 1.0f;
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf4, 0x0(%2)\n\t"
            "lqc2 $vf5, 0x10(%2)\n\t"
            "lqc2 $vf6, 0x20(%2)\n\t"
            "lqc2 $vf7, 0x30(%2)\n\t"
            "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
            "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
            "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
            "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(t0), "r"(t0), "r"(m) : "memory");
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vsub.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(t0), "r"(arg1), "r"(t0) : "memory");
        d = VuVecDot(SPR(0), SPR(0));
        r = arg2 + p->unk20;
        if (d <= r * r) {
            fldCollMeshHitCount++;
            idx = p->idx;
            verts = p->verts;
            for (j = 0; j < p->count; j++) {
                copyq(t4, verts + idx[0]);
                idx++;
                __asm__ volatile(
                    "lqc2 $vf1, 0(%1)\n\t"
                    "lqc2 $vf2, 0(%2)\n\t"
                    "vsub.xyzw $vf1, $vf1, $vf2\n\t"
                    "sqc2 $vf1, 0(%0)"
                    : : "r"(SPR(6)), "r"(verts + idx[0]), "r"(t4) : "memory");
                idx++;
                __asm__ volatile(
                    "lqc2 $vf1, 0(%1)\n\t"
                    "lqc2 $vf2, 0(%2)\n\t"
                    "vsub.xyzw $vf1, $vf1, $vf2\n\t"
                    "sqc2 $vf1, 0(%0)"
                    : : "r"(SPR(5)), "r"(verts + idx[0]), "r"(t4) : "memory");
                idx += 2;
                do { /* do-while(0): loop-depth weight for ok and the counter; the saved registers depend on it */
                    ok = VuSphereTriBoundHit(SPR(2), arg2, SPR(4));
                    fldCollTriTestCount++;
                } while (0);
                if (ok) {
                    ok = VuTriPlaneNear(SPR(2), arg2, SPR(4));
                    fldCollTriBoundCount++;
                    if (ok) {
                        fldCollTriPlaneCount++;
                        if (VuPointInTriangle(SPR(2), arg2, SPR(4))) {
                            fldCollTriHitCount++;
                            MapPolyAddExt(SPR(4));
                        }
                    }
                }
                cnt++;
            }
        }
    }
    fldCollTriTestTotal += cnt;
    return 1;
}
