#include "common.h"
#include "camera.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void CamAnglesToDir(Camera *c) {
    f32 v[4];
    f32 k = 10430.378f;
    f32 b = c->pitch;

    VuSinCosAngle(&v[0], &v[1], (s32)(c->yaw * k));
    VuSinCosAngle(&v[2], &v[3], (s32)(b * k));
    c->dir[0] = -v[0] * v[3];
    c->dir[1] = -v[2];
    c->dir[2] = v[1] * v[3];
}
