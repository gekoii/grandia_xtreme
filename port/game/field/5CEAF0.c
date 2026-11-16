/* Port-only C for CamPresetChaseMove (0x005CEAF0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"
#include "field/n15.h"

/* The swept sphere FldCollSweep takes */
typedef struct CamSweep {
    /* 0x00 */ s32 hit;
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ f32 radius;
    /* 0x10 */ FVECTOR pos;     /* start */
    /* 0x20 */ FVECTOR move;
    /* 0x30 */ FVECTOR stop;    /* move up to the first hit */
    /* 0x40 */ FVECTOR slide;   /* written by the map border sweep (MapBorderSweepWall,
                                   MapBorderSweepCircle); the game's frame had spare stack
                                   there, the port's frame has saved registers */
} CamSweep;

extern CamFollowWork *camPresetCur;
extern f32 camPresetDistance;

f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);
s32 CamPresetChaseMove(f32 *out, f32 *eye, f32 *at);

/* The VU0 forms: xyz from the operation, w from the first operand */
static void vsub(f32 *d, f32 *a, f32 *b) {
    f32 w = a[3];

    d[0] = a[0] - b[0];
    d[1] = a[1] - b[1];
    d[2] = a[2] - b[2];
    d[3] = w;
}

static void vadd(f32 *d, f32 *a, f32 *b) {
    f32 w = a[3];

    d[0] = a[0] + b[0];
    d[1] = a[1] + b[1];
    d[2] = a[2] + b[2];
    d[3] = w;
}

static void vscale(f32 *v, f32 k) {
    v[0] = v[0] * k;
    v[1] = v[1] * k;
    v[2] = v[2] * k;
}

static s32 sweep(CamSweep *s, f32 *from, f32 *move, f32 radius) {
    __builtin_memcpy(s->move, move, 16);
    __builtin_memcpy(s->stop, move, 16);
    __builtin_memcpy(s->pos, from, 16);
    s->radius = radius;
    s->hit = 0;
    return FldCollSweep((s32 *)s);
}

/* CamFollowClearChase (0x005CDCB0) for the preset camera: camPresetCur,
   camPresetDistance and CamPresetApproachSpeed, the last target position
   in save.lookAt, and a moving target's eye swept back from it. Ors 1
   (pulling in), 2 (closing in) or 3 (blocked) into camPresetState and
   returns it. */
s32 CamPresetChaseMove(f32 *out, f32 *eye, f32 *at) {
    FVECTOR v;
    FVECTOR saved;
    FVECTOR p;
    CamSweep s;
    f32 rate = camPresetCur->rate[2];
    f32 fastMax = rate * 20.0f;
    f32 fast = rate + rate;
    f32 slow = rate * 0.5f;
    f32 slowMax = rate * 10.0f;
    f32 spd = 0.0f;
    f32 moved;
    f32 cur;
    f32 d;
    f32 a;
    s32 i;
    s32 state = 0;

    __builtin_memcpy(saved, camPresetCur->unk90, 16);
    vsub(v, at, camPresetCur->save.lookAt);
    moved = __builtin_sqrtf(VuVecDot(v, v));
    cur = __builtin_sqrtf(VuVecDot(camPresetCur->unkA0, camPresetCur->unkA0));
    if (0.01f < __builtin_fabsf(moved)) {
        /* the target moved: follow it at camPresetDistance, as far as the
           way back from it is clear */
        vsub(v, saved, at);
        VuVecNormalize(v, v);
        vscale(v, camPresetDistance);
        vadd(p, v, at);
        vsub(out, p, eye);
        spd = CamPresetApproachSpeed(VuVecNormalizeLen(out, out), cur, fast, fastMax);
        vscale(out, spd);
        vadd(v, out, eye);
        vsub(v, v, at);
        sweep(&s, at, v, 10.0f);
        for (i = 0; i < 4; i++) {
            saved[i] = s.stop[i] + at[i];
        }
        for (i = 0; i < 4; i++) {
            out[i] = saved[i] - eye[i];
        }
    } else {
        /* the target stands still */
        vsub(v, at, saved);
        v[3] = at[3] - saved[3];
        d = __builtin_sqrtf(VuVecDot(v, v));
        vsub(v, saved, at);
        v[3] = saved[3] - at[3];
        d = VuVecNormalizeLen(v, v);
        vscale(v, camPresetDistance - 1.0f);
        vadd(p, at, v);
        __builtin_memset(out, 0, 12);
        out[3] = 1.0f;
        vsub(v, p, eye);
        if (__builtin_fabsf(__builtin_sqrtf(VuVecDot(v, v))) < 0.1f) {
            __builtin_memset(out, 0, 12);
            out[3] = 1.0f;
        } else if (camPresetDistance < d) {
            /* too far: pull in */
            vsub(v, saved, at);
            VuVecNormalize(v, v);
            vscale(v, camPresetDistance);
            vadd(v, at, v);
            vsub(out, v, eye);
            spd = CamPresetApproachSpeed(VuVecNormalizeLen(out, out), cur, slow, slowMax);
            vscale(out, spd);
            state = 1;
        } else {
            /* too close: back off unless something is right behind */
            vsub(out, eye, at);
            a = VuVecNormalizeLen(v, out) - 6.0f;
            vscale(v, a);
            sweep(&s, at, v, 15.0f);
            if (s.hit != 0) {
                __builtin_memset(out, 0, 12);
                out[3] = 1.0f;
                state = 3;
            } else {
                vsub(v, saved, at);
                VuVecNormalizeLen(v, v);
                vscale(v, camPresetDistance);
                sweep(&s, at, v, 10.0f);
                a = VuVecNormalizeLen(v, s.stop) - 1.0f;
                vscale(v, a);
                vadd(p, at, v);
                vsub(out, p, eye);
                spd = CamPresetApproachSpeed(VuVecNormalizeLen(out, out), cur, slow, slowMax);
                vscale(out, spd);
                state = 2;
            }
        }
    }
    camPresetCur->speed = spd;
    camPresetState |= state;
    return camPresetState;
}
