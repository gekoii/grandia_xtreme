/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVEC[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVEC pos;
} Mdl;

typedef struct {
    u8 pad0[6];
    u16 x6;
    u8 pad8[4];
    u16 xC;
    s16 xE;
    u8 pad10[8];
    u8 x18;
    u8 pad19[0x27];
    f32 x40;
    f32 x44;
    u8 pad48[0xC];
    s32 x54;
    s32 x58;
    s8 x5C;
    s8 x5D;
    u8 pad5E[2];
    Mdl *x60;
    u8 pad64[0x11C];
    FVEC x180;
    FVEC x190;
    FVEC x1A0;
    u8 pad1B0[0x10];
    f32 x1C0[4];
    u8 pad1D0[0x54];
    u16 x224;
    u8 pad226[2];
    u8 x228[4];
} Actor;

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void VuVecNormalize(FVEC, FVEC);
void VuVecLerp(FVEC, FVEC, FVEC, f32);
void VuBezier3(FVEC, FVEC *, f32);
void MapCollMove(s32, Mdl *, FVEC, void *, f32, u16, u16);

s32 ChrTenantOpFlyRandomDir(void *arg0, Actor *a, s32 arg2, s32 arg3) {
    FVEC k[4];
    FVEC out;
    FVEC v;
    f32 f;
    f32 g;
    s32 r;

    if (a->x5D != 0x15) {
        a->x5D = 0x15;
        ChrMotSet(a, 1);
        a->x40 = 1.0f;
        a->x44 = 0.02f;
        a->xE = arg3;
    }
    if (a->xE != 0) {
        a->xE--;
    }
    if (a->x40 >= 1.0f) {
        if (a->xE == 0) {
            if (a->xC & 0x40) {
                a->xC ^= 0x40;
            }
            return 0;
        }
        a->x40 = 0.0f;
        ChrGetByNo(1);
        a->xC |= 0x40;
        a->x58 = VuRandInt(0x8000);
        copyq(a->x190, a->x60->pos);
        f = arg2;
        ChrTenantAngleToVec(f, 90.0f, a->x58, (struct VuVec3 *)v);
        VuVecNormalize(v, v);
        f *= 100.0f;
        a->x1A0[1] = 0.0f;
        a->x1A0[0] = v[0] * f;
        a->x1A0[2] = v[2] * f;
        __asm__ volatile(
            "lqc2 $vf1, 0(%0)\n\t"
            "lqc2 $vf2, 0(%1)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(a->x1A0), "r"(a->x190) : "memory");
        a->x18 = VuRandInt(4);
        a->x1C0[0] = VuRandSigned(400.0f);
        a->x1C0[1] = VuRandSigned(400.0f);
        a->x1C0[2] = VuRandSigned(400.0f);
        a->x1C0[3] = VuRandSigned(400.0f);
        if (ChrTenantIsOutsideArea(a, a->x1A0) == 1) {
            a->x40 = 1.0f;
            return 0;
        }
    }
    a->x40 += a->x44;
    if (a->x40 > 1.0f) {
        a->x40 = 1.0f;
    }
    copyq(k[0], a->x190);
    copyq(k[3], a->x1A0);
    switch (a->x18) {
    case 0:
        VuVecLerp(k[1], a->x190, a->x1A0, 0.5f);
        copyq(k[2], k[1]);
        break;
    case 1:
        copyq(k[1], k[0]);
        copyq(k[2], k[3]);
        break;
    case 2:
        copyq(k[1], k[0]);
        copyq(k[2], k[0]);
        break;
    case 3:
        copyq(k[1], k[3]);
        copyq(k[2], k[3]);
        break;
    }
    g = arg3 * 20.5f;
    k[1][0] += a->x1C0[0];
    k[1][2] += a->x1C0[1];
    k[2][0] += a->x1C0[2];
    k[2][2] += a->x1C0[3];
    k[1][1] += g;
    k[2][1] += g;
    VuBezier3(out, k, a->x40);
    a->x54 = FldCalcYaw(out, a->x60->pos);
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(v), "r"(out), "r"(a->x60->pos) : "memory");
    copyq(a->x180, v);
    MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, 40.0f, a->x224, a->x6);
    r = 0;
    if (v[0] != a->x180[0] && v[2] != a->x180[2]) {
        r = 1;
    }
    if (v[0] < a->x180[0]) {
        r = 1;
    }
    if (r) {
        __asm__ volatile(
            "lqc2 $vf1, 0(%0)\n\t"
            "lqc2 $vf2, 0(%1)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(a->x60->pos), "r"(a->x180) : "memory");
        a->x40 = 1.0f;
    } else {
        copyq(a->x60->pos, out);
    }
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(a->x180) : "memory");
    return 1;
}
