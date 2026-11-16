/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
} Obj;

typedef struct {
    u8 pad0[0x6];
    u16 x6;
    u8 pad8[0x4];
    u16 xC;
    s16 xE;
    u8 pad10[0x8];
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
    Obj *x60;
    u8 pad64[0x11C];
    FVECTOR x180;
    FVECTOR x190;
    FVECTOR x1A0;
    u8 pad1B0[0x10];
    f32 x1C0;
    f32 x1C4;
    f32 x1C8;
    f32 x1CC;
    u8 pad1D0[0x54];
    u16 x224;
    u8 pad226[2];
    u8 x228[4];
} Actor;


void VuVecNormalize(f32 *, f32 *);
void VuVecLerp(f32 *, f32 *, f32 *, f32);
void VuBezier3(FVECTOR *, FVECTOR *, f32);
void MapCollMove(s32, Obj *, f32 *, void *, f32, s32, s32);

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define VADD(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSUB(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

#define VUNIT(d) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(d) : "memory")

s32 ChrTenantOpFlyToLeader(void *arg0, Actor *a, s32 r, s32 time)
{
    FVECTOR k[4];
    FVECTOR out;
    FVECTOR d;
    Actor *c;
    f32 s;
    f32 f;
    s32 flag;

    if (a->x5D != 20) {
        a->x5D = 20;
        ChrMotSet(a, 1);
        a->x40 = 1.0f;
        a->x44 = 0.02f;
        a->xE = time;
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
        c = ChrGetByNo(1);
        a->xC |= 0x40;
        copyq(a->x190, a->x60->pos);
        VSUB(out, c->x60->pos, a->x60->pos);
        VuVecNormalize(out, out);
        s = (f32)r * 100.0f;
        a->x1A0[0] = out[0] * s;
        a->x1A0[1] = out[1] * s;
        a->x1A0[2] = out[2] * s;
        VADD(a->x1A0, a->x1A0, a->x190);
        a->x18 = VuRandInt(4);
        a->x1C0 = VuRandSigned(400.0f);
        a->x1C4 = VuRandSigned(400.0f);
        a->x1C8 = VuRandSigned(400.0f);
        a->x1CC = VuRandSigned(400.0f);
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
    f = (f32)time * 20.5f;
    k[1][0] += a->x1C0;
    k[1][2] += a->x1C4;
    k[2][0] += a->x1C8;
    k[2][2] += a->x1CC;
    k[1][1] += f;
    k[2][1] += f;
    VuBezier3(&out, k, a->x40);
    a->x54 = FldCalcYaw(out, a->x60->pos);
    VSUB(d, out, a->x60->pos);
    copyq(a->x180, d);
    MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, 40.0f, a->x224, a->x6);
    flag = 0;
    if (d[0] != a->x180[0] && d[2] != a->x180[2]) {
        flag = 1;
    }
    if (d[0] < a->x180[0]) {
        flag = 1;
    }
    if (flag) {
        VADD(a->x60->pos, a->x60->pos, a->x180);
        a->x40 = 1.0f;
    } else {
        copyq(a->x60->pos, out);
    }
    VUNIT(a->x180);
    return 1;
}
