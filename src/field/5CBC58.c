#include "common.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void CamPitchYawToDir(f32 *out, f32 a, f32 b) {
    f32 v[4];
    f32 k = 10430.378f;

    VuSinCosAngle(&v[0], &v[1], (s32)(b * k));
    VuSinCosAngle(&v[2], &v[3], (s32)(a * k));
    out[0] = -v[0] * v[3];
    out[1] = -v[2];
    out[2] = v[1] * v[3];
}
