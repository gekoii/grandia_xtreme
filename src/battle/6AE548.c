/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"
#include "protos/battle.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void BtlCamBlend(void) {
    BtlCamCtrl *c = &btlCamCtrl;
    f32 sc[2];
    f32 one, r, h;
    s32 d;
    s32 e;

    if (c->duration == 0) {
        btlCamParam.target[0] = c->to.target[0];
        btlCamParam.target[1] = c->to.target[1];
        btlCamParam.target[2] = c->to.target[2];
        btlCamParam.eye[0] = c->to.eye[0];
        btlCamParam.eye[1] = c->to.eye[1];
        btlCamParam.eye[2] = c->to.eye[2];
        btlCamParam.rot[0] = c->to.rot[0];
        btlCamParam.rot[1] = c->to.rot[1];
        btlCamParam.rot[2] = c->to.rot[2];
        btlCamParam.dist = c->to.dist;
        btlCamParam.fov = c->to.fov;
        return;
    }
    one = 1.0f;
    r = one - powf((f32)btlCamCtrl.frames / (f32)c->duration, 3.0f);
    VuSinCosAngle(&sc[0], &sc[1], (f32)btlCamCtrl.frames / (f32)c->duration * 32768.0f);
    h = (sc[1] + one) * 0.5f;
    btlCamParam.rot[0] = c->from.rot[0] + (c->to.rot[0] - c->from.rot[0]) * r;
    btlCamParam.dist = c->from.dist + (c->to.dist - c->from.dist) * r;
    btlCamParam.fov = c->from.fov + (c->to.fov - c->from.fov) * r;
    btlCamParam.target[0] = c->from.target[0] + (c->to.target[0] - c->from.target[0]) * r;
    btlCamParam.target[1] = c->from.target[1] + (c->to.target[1] - c->from.target[1]) * h;
    btlCamParam.target[2] = c->from.target[2] + (c->to.target[2] - c->from.target[2]) * r;
    btlCamParam.eye[0] = c->from.eye[0] + (c->to.eye[0] - c->from.eye[0]) * r;
    btlCamParam.eye[1] = c->from.eye[1] + (c->to.eye[1] - c->from.eye[1]) * h;
    btlCamParam.eye[2] = c->from.eye[2] + (c->to.eye[2] - c->from.eye[2]) * r;
    d = (c->to.rot[1] - c->from.rot[1]) & 0xFFFF;
    if (d <= 0x7FFF) {
        btlCamParam.rot[1] = c->from.rot[1] + d * h;
    } else {
        btlCamParam.rot[1] = c->from.rot[1] - (0x10000 - d) * h;
    }
    e = (c->to.rot[2] - c->from.rot[2]) & 0xFFFF;
    if (e <= 0x7FFF) {
        btlCamParam.rot[2] = c->from.rot[2] + e * r;
    } else {
        btlCamParam.rot[2] = c->from.rot[2] - (0x10000 - e) * r;
    }
    if (--c->frames <= 0) {
        c->duration = 0;
    }
}
