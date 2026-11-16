/* as: ee-gcc2.9-991111-01 */
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
    u8 pad10[0x30];
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
    u8 pad1B0[0x74];
    u16 x224;
    u8 pad226[2];
    u8 x228[4];
} Actor;

extern f32 chrTenantJumpStepLarge;
extern f32 chrTenantJumpStepMedium;
extern f32 chrTenantJumpStepSmall;
extern f32 chrTenantJumpHeightLarge;
extern f32 chrTenantJumpHeightMedium;
extern f32 chrTenantJumpHeightSmall;

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

s32 ChrTenantOpHopRandomDir(void *arg0, Actor *a, s32 r, s32 time)
{
    FVECTOR k[4];
    FVECTOR out;
    FVECTOR dir;
    f32 s;
    f32 e;
    s32 flag;

    if (a->x5D != 10) {
        a->x5D = 10;
        ChrMotSet(a, 1);
        a->xE = time;
        a->x40 = 1.0f;
        if (time >= 11) {
            e = chrTenantJumpStepLarge;
        } else if (time >= 6) {
            e = chrTenantJumpStepMedium;
        } else {
            e = chrTenantJumpStepSmall;
        }
        a->x44 = e;
    }
    if (a->xE != 0) {
        a->xE--;
    }
    if (a->x40 >= 1.0f) {
        if (a->xE == 0) {
            a->xC |= 8;
            if (a->xC & 0x40) {
                a->xC ^= 0x40;
            }
            return 0;
        }
        a->x40 = 0.0f;
        ChrGetByNo(1);
        if (a->xC & 8) {
            a->xC ^= 8;
        }
        a->xC |= 0x40;
        a->x58 = a->x54 + (s32)VuRandSigned(16384.0f);
        copyq(a->x190, a->x60->pos);
        ChrTenantAngleToVec((f32)r, 90.0f, (f32)a->x58, dir);
        VuVecNormalize(dir, dir);
        s = (f32)r * 100.0f;
        a->x1A0[1] = 0.0f;
        a->x1A0[0] = dir[0] * s;
        a->x1A0[2] = dir[2] * s;
        VADD(a->x1A0, a->x1A0, a->x190);
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
    VuVecLerp(k[1], a->x190, a->x1A0, 0.5f);
    copyq(k[2], k[1]);
    if (time >= 11) {
        k[1][1] += (f32)time * chrTenantJumpHeightLarge;
        k[2][1] += (f32)time * chrTenantJumpHeightLarge;
    } else if (time >= 6) {
        k[1][1] += (f32)time * chrTenantJumpHeightMedium;
        k[2][1] += (f32)time * chrTenantJumpHeightMedium;
    } else {
        k[1][1] += (f32)time * chrTenantJumpHeightSmall;
        k[2][1] += (f32)time * chrTenantJumpHeightSmall;
    }
    VuBezier3(&out, k, a->x40);
    a->x54 = FldCalcYaw(out, a->x60->pos);
    VSUB(dir, out, a->x60->pos);
    copyq(a->x180, dir);
    MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, 40.0f, a->x224, a->x6);
    flag = 0;
    if (dir[0] != a->x180[0] && dir[2] != a->x180[2]) {
        flag = 1;
    }
    if (dir[0] < a->x180[0]) {
        flag = 1;
    }
    if (flag) {
        VADD(a->x60->pos, a->x60->pos, a->x180);
        a->x40 = 1.0f;
        return 0;
    }
    copyq(a->x60->pos, out);
    VUNIT(a->x180);
    return 1;
}
