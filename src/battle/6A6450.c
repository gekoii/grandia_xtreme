#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"

f32 VuSinAngle(s32);

void BtlStartCamSurprised(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    s32 t;

    switch (arg1->phase) {
    case 0:
        BtlCamSetCalcMode(2);
        arg1->count = 0;
        arg0->to.rot[2] = 0x8000;
        arg0->to.target[1] = 40.0f;
        arg0->to.eye[1] = 640.0f;
        arg0->to.eye[2] = 1.0f;
        arg1->phase++;
        arg0->to.target[0] = 0.0f;
        arg0->to.target[2] = 0.0f;
        arg0->to.eye[0] = 0.0f;
    case 1:
        t = arg1->count;
        arg1->count = t + 1;
        arg0->to.eye[1] = t * 6.25f + 640.0f;
        if (arg1->count >= 0x20) {
            arg1->count = 0;
            arg1->phase++;
        }
        break;
    case 2:
        arg0->to.eye[1] = VuSinAngle(arg1->count * 0xE3) * 1200.0f + 840.0f;
        arg0->to.eye[2] = arg1->count * (700.0f / 72.0f) + 1.0f;
        if (arg1->count < 0x49) {
            arg0->to.rot[2] = VuCosAngle(arg1->count * 0x1C7) * 16384.0f + 16384.0f;
        }
        arg1->count++;
        if (arg1->count >= 0xA0) {
            BtlCamSetMode(1, 0x40, 0, NULL);
        }
        break;
    }
}
