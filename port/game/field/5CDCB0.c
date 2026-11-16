/* Port-only C for CamFollowClearChase (0x005CDCB0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"
#include "field/n15.h"
#include "grx_camera.h"

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

extern f32 camFollowDistance;

f32 VuVecDot(f32 *, f32 *);
void VuVecCopy(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);
s32 CamFollowClearChase(f32 *out, f32 *eye, f32 *at);

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

/* out = the eye's move this frame when the way is clear: back to
   camFollowDistance behind `at` along the saved eye (unk90), with the
   approach speed saved in camFollowCur->speed. Ors 1 (pulling in), 2
   (closing in) or 3 (blocked) into camFollowState and returns it. */
s32 CamFollowClearChase(f32 *out, f32 *eye, f32 *at) {
    FVECTOR v;
    FVECTOR saved;
    FVECTOR p;
    CamSweep s;
    f32 rate = camFollowCur->rate[2];
    f32 fastMax = rate * 20.0f;
    f32 fast = rate + rate;
    f32 slow = rate * 0.5f;
    f32 slowMax = rate * 10.0f;
    f32 spd = 0.0f;
    f32 moved;
    f32 cur;
    f32 d;
    f32 a;
    f32 pitch;
    s32 state = 0;

    __builtin_memcpy(saved, camFollowCur->unk90, 16);
    vsub(v, at, camFollowCur->unk50);
    moved = __builtin_sqrtf(VuVecDot(v, v));
    cur = __builtin_sqrtf(VuVecDot(camFollowCur->unkA0, camFollowCur->unkA0));
    if (0.01f < __builtin_fabsf(moved)) {
        /* the target moved: follow it at camFollowDistance */
        vsub(v, saved, at);
        VuVecNormalize(v, v);
        vscale(v, camFollowDistance);
        vadd(p, v, at);
        vsub(out, p, eye);
        spd = CamApproachSpeed(VuVecNormalizeLen(out, out), cur, fast, fastMax);
        vscale(out, spd);
        VuVecCopy(v, out);
        MapCollMoveCamera(eye, out, 11.0f);
        vadd(saved, eye, out);
        vsub(v, saved, at);
        d = __builtin_sqrtf(VuVecDot(v, v));
        if (camFollowDistance < d) {
            goto end;
        }
        a = CamVecPitch(v);
        pitch = camFollowCur->pitch;
        if (a < pitch) {
            goto end;
        }
        if (pitch < a && out[1] < 0.0f) {
            goto end;
        }
        /* below the pitch limit: lift the eye to it. The game has
           d * asinf(pitch) (pitch in radians), near d * sin(pitch) at its
           own 8 and 20 degrees; at a pitch over 1 (57 degrees: the stick's
           tilt, or a floor pushing the camera up) it is NaN, which the
           walking camera never recovers from (grx_cam_lift) */
        saved[1] = at[1] + d * grx_cam_lift(pitch);
        vsub(v, saved, at);
        sweep(&s, at, v, 11.0f);
        VuVecCopy(v, s.stop);
        vadd(saved, at, v);
        vsub(out, saved, eye);
        goto end;
    }
    /* the target stands still */
    vsub(v, at, saved);
    v[3] = at[3] - saved[3];
    d = __builtin_sqrtf(VuVecDot(v, v));
    vsub(v, saved, at);
    v[3] = saved[3] - at[3];
    d = VuVecNormalizeLen(v, v);
    vscale(v, camFollowDistance - 1.0f);
    vadd(p, at, v);
    __builtin_memset(out, 0, 12);
    out[3] = 1.0f;
    vsub(v, p, eye);
    if (__builtin_fabsf(__builtin_sqrtf(VuVecDot(v, v))) < 0.1f) {
        __builtin_memset(out, 0, 12);
        out[3] = 1.0f;
    } else if (camFollowDistance < d) {
        /* too far: pull in */
        vsub(v, saved, at);
        VuVecNormalize(v, v);
        vscale(v, camFollowDistance);
        vadd(v, at, v);
        vsub(out, v, eye);
        spd = CamApproachSpeed(VuVecNormalizeLen(out, out), cur, slow, slowMax);
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
            vscale(v, camFollowDistance);
            sweep(&s, at, v, 10.0f);
            a = VuVecNormalizeLen(v, s.stop) - 1.0f;
            vscale(v, a);
            vadd(p, at, v);
            vsub(out, p, eye);
            spd = CamApproachSpeed(VuVecNormalizeLen(out, out), cur, slow, slowMax);
            vscale(out, spd);
            state = 2;
        }
    }
end:
    camFollowCur->speed = spd;
    camFollowState |= state;
    return camFollowState;
}
