/* Port-only C for CamCalcMatrix (0x00431380), which the decompilation still has in assembly. */
#include "common.h"
#include "camera.h"
#include "protos/main.h"
#include "vu0_port.h"

void CamCalcDir(Camera *cam);
void CamCalcUp(Camera *cam);
void CamCalcRoll(Camera *cam);
void VuVecCopy(f32 *dst, f32 *src);
void VuMtxMul(f32 *dst, f32 *a, f32 *b);

void CamCalcMatrix(Camera *cam);

/* viewMtx[3].xyz += v.xyz */
static void AddTranslation(Camera *cam, const f32 *v) {
    cam->viewMtx[3][0] += v[0];
    cam->viewMtx[3][1] += v[1];
    cam->viewMtx[3][2] += v[2];
}

/*
 * Derives what mode doesn't hold (bit 0 lookAt, 1 eye, 2 dir, 3 angles,
 * 0x20 roll, 0x40 up are "given"), then builds the view matrix:
 * translate by -(lookAt [+ shakeOfs]), rotate by -yaw, -pitch, -roll,
 * translate by (0, 0, dist), and by shakeOfs when shakeMode & 4.
 */
void CamCalcMatrix(Camera *cam) {
    FVECTOR v;
    FVECTOR d;
    FMATRIX rot;
    s32 mode = cam->mode;
    s32 i;

    if ((mode & 3) == 3 && !(mode & 4)) {
        CamCalcDir(cam);
        mode |= 0x14;
    }
    if (mode & 4) {
        if (!(mode & 8)) {
            CamDirToAngles(cam);
            mode |= 8;
        }
    } else if (mode & 8) {
        CamAnglesToDir(cam);
        mode |= 4;
    }
    if (!(mode & 2)) {
        cam->eye[3] = 1.0f;
        cam->eye[0] = -cam->dir[0] * cam->dist + cam->lookAt[0];
        cam->eye[1] = -cam->dir[1] * cam->dist + cam->lookAt[1];
        cam->eye[2] = -cam->dir[2] * cam->dist + cam->lookAt[2];
    }
    if (!(mode & 1)) {
        cam->lookAt[0] = cam->dir[0] * cam->dist + cam->eye[0];
        cam->lookAt[1] = cam->dir[1] * cam->dist + cam->eye[1];
        cam->lookAt[2] = cam->dir[2] * cam->dist + cam->eye[2];
        cam->lookAt[3] = 1.0f;
    }
    if (!(mode & 0x20)) {
        CamCalcRoll(cam);
    }
    if (!(mode & 0x40)) {
        CamCalcUp(cam);
    }

    VuUnitMtx(&cam->viewMtx[0][0]);
    if (cam->shakeMode & 4) {
        VuVecCopy(v, cam->lookAt);
    } else {
        VuCopy4(v, cam->lookAt);
        for (i = 0; i < 3; i++) {
            v[i] = cam->lookAt[i] + cam->shakeOfs[i];
        }
    }
    for (i = 0; i < 3; i++) {
        v[i] = v[i] * -1.0f;
    }
    v[3] = 1.0f;
    AddTranslation(cam, v);

    VuUnitMtx(&rot[0][0]);
    VuMtxRotY((s32)rot, VuFtoi(-(cam->yaw * VuBits(0x4622F983)), 0), (s32)rot);
    VuMtxRotX((s32)rot, VuFtoi(-(cam->pitch * VuBits(0x4622F983)), 0), (s32)rot);
    VuMtxRotZ((s32)rot, VuFtoi(-(cam->roll * VuBits(0x4622F983)), 0), (s32)rot);
    VuMtxMul(&cam->viewMtx[0][0], &rot[0][0], &cam->viewMtx[0][0]);

    d[0] = 0.0f;
    d[1] = 0.0f;
    d[2] = cam->dist;
    d[3] = cam->dist;
    AddTranslation(cam, d);
    if (cam->shakeMode & 4) {
        VuVecCopy(d, cam->shakeOfs);
        d[3] = 1.0f;
        AddTranslation(cam, d);
    }
}
