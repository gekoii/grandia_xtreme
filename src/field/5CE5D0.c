/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    f32 unk10;
    u8 pad14[0x10];
    f32 unk24;
    u8 pad28[0x28];
    FVECTOR unk50;
    FVECTOR unk60;
    u8 pad70[0x20];
    FVECTOR unk90;
    FVECTOR unkA0;
    u8 padB0[0x30];
    FVECTOR unkE0;
    u8 padF0[0x10];
    f32 unk100;
    u8 pad104[0xC];
    FVECTOR unk110;
    u8 pad120[0x28];
    f32 unk148;
} Cam;

typedef struct {
    u8 pad0[0x50];
    FVECTOR unk50;
    FVECTOR unk60;
    u8 pad70[0x30];
    s32 unkA0;
    u8 padA4[0xAC];
    f32 unk150;
} Obj;

typedef struct {
    s32 unk0;
    s32 pad4[2];
    f32 unkC;
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
    u8 pad40[0x40];
} Look;

void VuVecCopy(void *, void *);
f32 VuVecDot(void *, void *);
void VuVecNormalize(void *, void *);

extern Cam *camPresetCur;
extern s32 camPresetUnusedB5C;
extern s32 camPresetPrevState;
extern s32 camPresetState;
extern s32 camPresetUnusedB68;
extern f32 camPresetMoveDot;
extern f32 camPresetDistance;
extern f32 camPresetMoveLen;
extern u8 camCtlSubType;

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) {
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

#define VSUB(d, a, b) \
    __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvsub.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" \
                     : : "r"(d), "r"(a), "r"(b) : "memory")
#define VADD(d, a, b) \
    __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvadd.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" \
                     : : "r"(d), "r"(a), "r"(b) : "memory")
#define VSCALE(d, a, k) \
    __asm__ volatile("lqc2 $vf1, 0(%1)\n\tqmtc2.ni %2, $vf2\n\tvmulx.xyz $vf1, $vf1, $vf2x\n\tsqc2 $vf1, 0(%0)" \
                     : : "r"(d), "r"(a), "r"(k) : "memory")

void CamAiPresetUpdate(Obj *obj, f32 *pos)
{
    Look look;
    FVECTOR dir;
    FVECTOR ofs;
    FVECTOR d;
    FVECTOR at;
    FVECTOR u;
    FVECTOR w;
    f32 a;
    f32 v;
    f32 len;
    f32 ten;

    camPresetCur = CamFollowGetWork();
    VSUB(d, pos, camPresetCur->unk50);
    a = __builtin_fabsf(d[1]);
    v = camPresetCur->unk10;
    if (a > 0.01f) {
        f32 dy = pos[1] - camPresetCur->unk50[1];
        if (d[1] < 0.0f) {
            dy = -dy;
        }
        v = CamPresetApproachSpeedRel(a, dy, v, 0.4f, 4.0f);
        if (d[1] > 0.0f) {
            d[1] = v;
        } else {
            d[1] = -v;
        }
        VADD(pos, d, camPresetCur->unk50);
        VuVecCopy(camPresetCur->unk110, pos);
    } else {
        v = 0.0f;
    }
    camPresetCur->unk10 = v;
    a = fsqrt(VuVecDot(d, d));
    CamCalcEyePos(pos, camPresetCur->unk90, camPresetCur->unk148, camPresetCur->unk24, camPresetCur->unk100);
    VuVecCopy(camPresetCur->unk60, pos);
    {
        Cam *c = camPresetCur;
        VuVecCopy(pos, c->unk110);
        VuVecCopy(at, c->unkE0);
    }
    VSUB(u, pos, camPresetCur->unk50);
    len = fsqrt(VuVecDot(u, u));
    camPresetMoveDot = 0.0f;
    camPresetMoveLen = len;
    if (len > 0.1f) {
        VuVecNormalize(u, u);
        VSUB(w, pos, at);
        VuVecNormalize(w, w);
        camPresetMoveDot = VuVecDot(u, w);
    }
    VSUB(u, pos, at);
    a = fsqrt(VuVecDot(u, u));
    VADD(u, pos, at);
    VSCALE(u, u, 0.5f);
    FldCollGather(u, a + 100.0f);
    VSUB(dir, at, pos);
    VSCALE(dir, dir, 0.9999f);
    ten = 10.0f;
    {
        f32 *p = dir;
        copyq(look.unk20, p);
        copyq(look.unk30, p);
    }
    copyq(look.unk10, pos);
    look.unkC = ten;
    look.unk0 = 0;
    switch (camCtlSubType) {
    case 0:
        camPresetCur->unk24 = 0.13962634f;
        camPresetDistance = 400.0f;
        FldCollSweep(&look);
        break;
    case 1:
    case 2:
        camPresetCur->unk24 = 1.5358897f;
        camPresetDistance = 200.0f;
        break;
    case 3:
        camPresetCur->unk24 = 0.13962634f;
        camPresetDistance = 200.0f;
        FldCollSweep(&look);
        break;
    case 4:
        camPresetCur->unk24 = 0.13962634f;
        camPresetDistance = 200.0f;
        FldCollSweep(&look);
        break;
    }
    CamCalcEyePos(pos, camPresetCur->unk90, camPresetCur->unk148, camPresetCur->unk24, camPresetCur->unk100);
    VADD(at, look.unk30, pos);
    VSUB(ofs, u, at);
    do {} while (0); /* scheduling barrier (4 words off without) */
    camPresetState = 0x100;
    CamTrailReset(pos, at);
    CamPresetChase(ofs, at, pos);
    obj->unk150 = 42.0f;
    do {} while (0); /* scheduling barrier: the fov store stays before the vector ops (2 words) */
    VADD(obj->unk60, ofs, at);
    VuVecCopy(obj->unk50, pos);
    VuVecCopy(camPresetCur->unkA0, ofs);
    VuVecCopy(camPresetCur->unk50, pos);
    obj->unkA0 = 0x23;
    CamApply(obj);
    camPresetPrevState = camPresetState;
    camPresetUnusedB68 = 0;
    camPresetUnusedB5C = 0;
}
