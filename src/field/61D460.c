/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR pos;
    FVECTOR dir;
    f32 len;
    f32 rad;
} Capsule;

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

void VuVecCopy(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
s32 VuCapsuleSphereHit(Capsule *, f32 *);
s32 VuCapsuleTriHit(Capsule *, f32 *);
/* Declared radius-second: the argument-load order only matches this way. */
s32 VuSphereTriBoundHit(f32 *, f32, f32 *);

#define SPR(n) ((f32 *)(0x70003000 + (n) * 16))

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

s32 FldCollPartGatherCapsule(Model *arg0, f32 *arg1, f32 *arg2, f32 arg3) {
    Capsule c;
    FVECTOR *m;
    Part *p;
    s32 i;
    s32 j;
    s32 cnt;
    s16 *idx;
    u8 *verts;
    f32 rad;
    f32 *t1;
    f32 *t2;
    f32 *t3;

    cnt = 0;
    t1 = SPR(1);
    t2 = SPR(2);
    t3 = SPR(3);
    m = arg0->mtx;
    p = arg0->parts;
    VuVecCopy(c.pos, arg1);
    c.len = VuVecNormalizeLen(c.dir, arg2);
    c.rad = arg3;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "qmtc2.ni %2, $vf2\n\t"
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(t1), "r"(arg2), "r"(0.5f) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(t2), "r"(arg1), "r"(t1) : "memory");
    rad = arg3 + fsqrt(VuVecDot(SPR(1), SPR(1)));
    for (i = 0; i < arg0->nparts; p++, i++) {
        copyq(t1, p->center);
        t1[3] = 1.0f;
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
            : : "r"(t1), "r"(t1), "r"(m) : "memory");
        t1[3] = p->unk20;
        if (VuCapsuleSphereHit(&c, SPR(1))) {
            fldCollMeshHitCount++;
            idx = p->idx;
            verts = p->verts;
            for (j = 0; j < p->count; j++) {
                copyq(t3, verts + idx[0]);
                idx++;
                __asm__ volatile(
                    "lqc2 $vf1, 0(%1)\n\t"
                    "lqc2 $vf2, 0(%2)\n\t"
                    "vsub.xyzw $vf1, $vf1, $vf2\n\t"
                    "sqc2 $vf1, 0(%0)"
                    : : "r"(SPR(5)), "r"(verts + idx[0]), "r"(t3) : "memory");
                idx++;
                __asm__ volatile(
                    "lqc2 $vf1, 0(%1)\n\t"
                    "lqc2 $vf2, 0(%2)\n\t"
                    "vsub.xyzw $vf1, $vf1, $vf2\n\t"
                    "sqc2 $vf1, 0(%0)"
                    : : "r"(SPR(4)), "r"(verts + idx[0]), "r"(t3) : "memory");
                idx += 2;
                do { /* empty: scheduling barrier, idx += 2 stays ahead of the call setup and the next label keeps its alignment nop */
                } while (0);
                if (VuSphereTriBoundHit(SPR(2), rad, SPR(3)) && VuCapsuleTriHit(&c, SPR(3))) {
                    __asm__ volatile(
                        "lqc2 $vf1, 0(%1)\n\t"
                        "lqc2 $vf2, 0(%2)\n\t"
                        "vopmula.xyz ACC, $vf1, $vf2\n\t"
                        "vopmsub.xyz $vf1, $vf2, $vf1\n\t"
                        "sqc2 $vf1, 0(%0)"
                        : : "r"(SPR(6)), "r"(SPR(5)), "r"(SPR(4)) : "memory");
                    VuVecNormalize(SPR(6), SPR(6));
                    if (!(VuVecDot(SPR(6), c.dir) > 0.0f)) {
                        MapPolyAddExt(SPR(3));
                    }
                }
                cnt++;
            }
        }
    }
    fldCollTriTestTotal += cnt;
    return 1;
}
