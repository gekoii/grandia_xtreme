/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

typedef struct {
    s32 unk0;
    u8 pad4[8];
    f32 unkC;
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
} Hit;

typedef struct {
    s32 unk0;
    u8 pad4[0xC];
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
    u8 pad40[0x40];
} Ray;

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define VSUB(d, a, b) __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvsub.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" : : "r"(d), "r"(a), "r"(b) : "memory")
#define VADD(d, a, b) __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvadd.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" : : "r"(d), "r"(a), "r"(b) : "memory")
#define VSCALE(d, a, k) __asm__ volatile("lqc2 $vf1, 0(%1)\n\tqmtc2.ni %2, $vf2\n\tvmulx.xyz $vf1, $vf1, $vf2x\n\tsqc2 $vf1, 0(%0)" : : "r"(d), "r"(a), "r"(k) : "memory")
#define VUNIT(d) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(d) : "memory")

void VuVecCopy(void *, void *);
f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

static inline s32 probe(Hit *h, f32 *pos, f32 *dir, f32 r) {
    copyq(h->unk20, dir);
    copyq(h->unk30, dir);
    copyq(h->unk10, pos);
    h->unkC = r;
    h->unk0 = 0;
    FldCollSweep((s32 *)h);
    return h->unk0;
}
extern f32 camFollowHeightSpeed;
extern Hit camRayQuery;
extern FVECTOR camRayQueryOut;
extern s32 camFollowReqFlag;
extern s32 camFollowBlockedFrames;
extern s32 camFollowPrevState;
extern s32 camFollowState;
extern s32 camFollowJustBlocked;
extern f32 camFollowMoveDot;
extern f32 camFollowDistance;
extern f32 camFollowMoveLen;

void CamAiFollowUpdate(Camera *obj, f32 *pos) {
    Ray ray;
    FVECTOR v80;
    FVECTOR v90;
    FVECTOR vA0;
    FVECTOR vB0;
    FVECTOR vC0;
    FVECTOR vD0;
    f32 ay;
    f32 h1;
    f32 len;
    f32 ang;
    f32 h2;
    s32 hit1;
    s32 hit2;
    s32 res;
    CamFollowWork *c;

    camFollowCur = CamFollowGetWork();
    VSUB(vA0, pos, camFollowCur->unk50);
    ay = fabsf(vA0[1]);
    camFollowHeightSpeed = camFollowCur->heightSpeed;
    if (ay > 0.01f) {
        f32 t;
        f32 r;
        t = pos[1] - camFollowCur->unk50[1];
        if (vA0[1] < 0.0f) {
            t = -t;
        }
        r = CamApproachSpeedRel(ay, t, camFollowHeightSpeed, 0.5f, 25.0f);
        camFollowHeightSpeed = r;
        if (vA0[1] > 0.0f) {
            vA0[1] = r;
        } else {
            vA0[1] = -r;
        }
        VADD(pos, vA0, camFollowCur->unk50);
        VuVecCopy(camFollowCur->save.lookAt, pos);
    } else {
        camFollowHeightSpeed = 0.0f;
    }
    camFollowCur->heightSpeed = camFollowHeightSpeed;
    ay = fsqrt(VuVecDot(vA0, vA0));
    CamCalcEyePos(pos, camFollowCur->unk90, camFollowCur->save.yaw, camFollowCur->pitch, camFollowCur->cur.dist);
    VuVecCopy(camFollowCur->unk60, pos);
    c = camFollowCur;
    VuVecCopy(pos, c->save.lookAt);
    VuVecCopy(vB0, c->cur.eye);
    VSUB(vC0, pos, camFollowCur->unk50);
    len = fsqrt(VuVecDot(vC0, vC0));
    camFollowMoveDot = 0.0f;
    camFollowMoveLen = len;
    if (len > 0.1f) {
        VuVecNormalize(vC0, vC0);
        VSUB(vD0, pos, vB0);
        VuVecNormalize(vD0, vD0);
        camFollowMoveDot = VuVecDot(vC0, vD0);
    }
    camFollowDistance = camFollowCur->dist[0];
    if (camFollowMoveDot < -0.5f) {
        camFollowDistance = camFollowCur->dist[1];
    }
    if (camFollowMoveDot > 0.5f) {
        camFollowDistance = camFollowCur->dist[2];
    }
    VSUB(vC0, pos, vB0);
    ay = fsqrt(VuVecDot(vC0, vC0));
    VADD(vC0, pos, vB0);
    VSCALE(vC0, vC0, 0.5f);
    FldCollGather(vC0, ay + 100.0f);
    /* scheduling barrier, then a do-while(0) wrap for allocation weight (without: 311 / 32 words off) */
    do {} while (0);
    do {
        ang = camFollowCur->pitch;
        VuVecCopy(ray.unk10, vB0);
    } while (0);
    VUNIT(ray.unk20);
    ray.unk20[1] = 120.0f;
    VuVecCopy(ray.unk30, ray.unk20);
    hit1 = FldCollTriListHitTest(&ray);
    h1 = ray.unk30[1];
    ray.unk20[1] = -150.0f;
    VuVecCopy(ray.unk30, ray.unk20);
    hit2 = FldCollTriListHitTest(&ray);
    h2 = -ray.unk30[1];
    /* try1 is placed after the other cases and falls back into them, as the
       original lays the blocks out (an if/else with one goto is 265 words off) */
    if (hit1 != 0) {
        goto try1;
    }
other:
    if (hit2 != 0) {
        if (h2 < 100.0f) {
            f32 r;
            camFollowCur->unk90[1] = vB0[1] + 100.0f - h2;
            VSUB(vC0, camFollowCur->unk90, pos);
            r = CamVecPitch(vC0);
            if (camFollowCur->pitch < r) {
                camFollowCur->pitch = r;
            }
        }
    } else {
        f32 t;
        f32 r;
        VSUB(vA0, vB0, pos);
        CamVecPitch(vA0);
        t = camFollowCur->unk30;
        if (ang < t) {
            r = ang + 0.006544985f;
            if (t < r) {
                r = t;
            }
        } else {
            r = ang - 0.006544985f;
            if (r < t) {
                r = t;
            }
        }
        camFollowCur->pitch = r;
    }
    goto done;
try1:
    if (!(h1 < 50.0f)) {
        goto other;
    }
    {
        f32 r;
        camFollowCur->unk90[1] = vB0[1] - 50.0f + h1;
        VSUB(vC0, camFollowCur->unk90, pos);
        r = CamVecPitch(vC0);
        if (!(r < camFollowCur->pitch)) {
            goto other;
        }
        camFollowCur->pitch = r;
    }
done:
    VSUB(v80, vB0, pos);
    VSCALE(v80, v80, 0.9999f);
    res = probe(&camRayQuery, pos, v80, 10.0f);
    if (res == 1) {
        camFollowBlockedFrames++;
    } else {
        camFollowBlockedFrames = 0;
    }
    if (res == 0) {
        ay = fsqrt(VuVecDot(v80, v80));
        v80[1] = ay * sinf(camFollowCur->pitch);
        if (probe(&camRayQuery, pos, v80, 10.0f) != 0) {
            VSUB(v80, vB0, pos);
            CamVecPitch(v80);
            camFollowCur->pitch = ang;
        }
    }
    /* do-while(0) wrap: allocation weight (unwrapped: 146 words off) */
    do {
    if (camFollowBlockedFrames < 181) {
        if (res != 0) {
            camFollowState = 0;
            if ((camFollowPrevState & 0xF00) == 0x100) {
                camFollowJustBlocked = 1;
            }
            if (CamTrailCount() == 0) {
                CamTrailPushFront(pos);
            } else {
                CamTrailUpdate(pos, vB0);
            }
            CamFollowBlocked(v90, vB0, pos);
        } else {
            camFollowState = 0x100;
            CamTrailReset(pos, vB0);
            CamFollowClear(v90, vB0, pos);
        }
    } else {
        VADD(vC0, pos, camRayQueryOut);
        VUNIT(v90);
        VuVecCopy(vB0, vC0);
    }
    } while (0);
    obj->fov = 42.0f;
    VADD(obj->eye, v90, vB0);
    VuVecCopy(obj->lookAt, pos);
    VuVecCopy(camFollowCur->unkA0, v90);
    VuVecCopy(camFollowCur->unk50, pos);
    obj->mode = 0x23;
    CamApply(obj);
    camFollowReqFlag = 0;
    camFollowJustBlocked = 0;
    camFollowPrevState = camFollowState;
}

