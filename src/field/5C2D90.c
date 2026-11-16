/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
} Mdl;

typedef struct {
    u8 pad0[0x60];
    Mdl *x60;
} Chara;

typedef struct {
    u8 pad0[6];
    u16 x6;
    u8 pad8[4];
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
    Mdl *x60;
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

void VuBezier3(f32 *, f32 *, f32);
void VuVecNormalize(f32 *, f32 *);
void VuVecLerp(f32 *, f32 *, f32 *, f32);
void MapCollMove(s32, Mdl *, f32, f32 *, void *, u16, u16);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

#define VSUB(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VADD(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

s32 ChrTenantOpHopToLeader(void *arg0, Actor *a, s32 dist, s32 lv)
{
    FVECTOR pts[4];
    FVECTOR v;
    FVECTOR d;
    Chara *c;
    f32 k;
    s32 r;

    if (a->x5D != 8) {
        a->x5D = 8;
        ChrMotSet(a, 1);
        a->xE = lv;
        a->x40 = 1.0f;
        if (lv >= 11) {
            a->x44 = chrTenantJumpStepLarge;
        } else if (lv >= 6) {
            a->x44 = chrTenantJumpStepMedium;
        } else {
            a->x44 = chrTenantJumpStepSmall;
        }
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
        c = ChrGetByNo(1);
        if (a->xC & 8) {
            a->xC ^= 8;
        }
        a->xC |= 0x40;
        copyq(a->x190, a->x60->pos);
        VSUB(v, c->x60->pos, a->x60->pos);
        VuVecNormalize(v, v);
        k = dist * 100.0f;
        a->x1A0[0] = v[0] * k;
        a->x1A0[1] = v[1] * k;
        a->x1A0[2] = v[2] * k;
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
    copyq(pts[0], a->x190);
    copyq(pts[3], a->x1A0);
    VuVecLerp(pts[1], a->x190, a->x1A0, 0.5f);
    copyq(pts[2], pts[1]);
    if (lv >= 11) {
        f32 k = lv * chrTenantJumpHeightLarge;
        pts[1][1] += k;
        pts[2][1] += k;
    } else if (lv >= 6) {
        f32 k = lv * chrTenantJumpHeightMedium;
        pts[1][1] += k;
        pts[2][1] += k;
    } else {
        f32 k = lv * chrTenantJumpHeightSmall;
        pts[1][1] += k;
        pts[2][1] += k;
    }
    VuBezier3(v, pts[0], a->x40);
    a->x54 = FldCalcYaw(v, a->x60->pos);
    VSUB(d, v, a->x60->pos);
    copyq(a->x180, d);
    if (a->x40 > 0.4f) {
        MapCollMove(MapCollModelGet(0), a->x60, 40.0f, a->x180, a->x228, a->x224, a->x6);
    }
    r = 0;
    if (d[0] != a->x180[0] && d[2] != a->x180[2]) {
        r = 1;
    }
    if (d[0] < a->x180[0]) {
        r = 1;
    }
    if (r) {
        VADD(a->x60->pos, a->x60->pos, a->x180);
        a->x40 = 1.0f;
        return 0;
    }
    copyq(a->x60->pos, v);
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(a->x180) : "memory");
    return 1;
}
