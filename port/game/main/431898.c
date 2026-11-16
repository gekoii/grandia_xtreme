/* Port-only C for CamCalcUp (0x00431898) and CamCalcRoll (0x00431998),
   which the decompilation still has in assembly (asm/main/main/031898.s).
   Their caller is CamCalcMatrix (port/game/main/431380.c). */
/*
 * Angles in radians become the 16-bit angles VuMtxRot* take by
 * cvt.w.s (truncating, saturating) of angle * 65536 / 2pi. Both build their rotation
 * in a fixed scratch matrix (camUpRotMatrix, camRollRotMatrix), as the asm
 * does, and leave $vf5-$vf7 as the identity's rows (vmr32 of $vf0); no
 * caller reads them, so the port doesn't write grx_vf.
 */
#include "common.h"
#include "camera.h"
#include "vu0_port.h"

extern u8 camUpRotMatrix[0x40];
extern u8 camRollRotMatrix[0x40];

void VuMtxRotX(f32 *dst, s32 angle, f32 *src);
void VuMtxRotY(f32 *dst, s32 angle, f32 *src);
void VuMtxRotZ(f32 *dst, s32 angle, f32 *src);
void VuVecNormalize(f32 *dst, f32 *src);
f32 VuVecDot(f32 *a, f32 *b);

void CamCalcUp(Camera *cam);
void CamCalcRoll(Camera *cam);

#define CAM_ANGLE_SCALE 0x4622F983u /* 10430.378 = 65536 / 2pi */

static s32 CamAngle(f32 rad) {
    return VuFtoi(rad * VuBits(CAM_ANGLE_SCALE), 0);
}

/* a.xyz x b.xyz (vopmula/vopmsub), w from a */
static void Cross(f32 *out, const f32 *a, const f32 *b) {
    f32 r[4];

    r[0] = a[1] * b[2] - b[1] * a[2];
    r[1] = a[2] * b[0] - b[2] * a[0];
    r[2] = a[0] * b[1] - b[0] * a[1];
    r[3] = a[3];
    VuCopy4(out, r);
}

/* up = Ry(yaw) Rx(pitch) Rz(roll) applied to (0, 1, 0, 0) */
void CamCalcUp(Camera *cam) {
    f32 *m = (f32 *)camUpRotMatrix;
    FVECTOR v;

    v[0] = 0.0f;
    v[1] = 1.0f;
    v[2] = 0.0f;
    v[3] = 0.0f;
    VuUnitMtx(m);
    VuMtxRotZ(m, CamAngle(cam->roll), m);
    VuMtxRotX(m, CamAngle(cam->pitch), m);
    VuMtxRotY(m, CamAngle(cam->yaw), m);
    VuApplyMtx(v, v, m);
    VuCopy4(cam->up, v);
}

/* Orthogonalises the up vector against the direction ((dir x up) x dir,
   normalised) and derives the roll: the dot product of that with the
   up of the pitch and yaw alone */
void CamCalcRoll(Camera *cam) {
    f32 *m = (f32 *)camRollRotMatrix;
    FVECTOR v;

    Cross(v, cam->dir, cam->up);
    Cross(cam->up, v, cam->dir);
    VuVecNormalize(cam->up, cam->up);
    VuUnitMtx(m);
    v[0] = 0.0f;
    v[1] = 1.0f;
    v[2] = 0.0f;
    v[3] = 0.0f;
    VuMtxRotX(m, CamAngle(cam->pitch), m);
    VuMtxRotY(m, CamAngle(cam->yaw), m);
    VuApplyMtx(v, v, m);
    cam->roll = VuVecDot(cam->up, v);
}
