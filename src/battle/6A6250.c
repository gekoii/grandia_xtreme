#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"


void BtlStartCamForestall(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    s32 t;
    f32 f;

    switch (arg1->phase) {
    case 0:
        arg0->to.rot[0] = 0x3D80;
        arg0->to.rot[1] = 0x1900;
        arg0->to.rot[2] = 0;
        arg0->to.dist = 1000.0f;
        arg0->to.target[0] = 333.0f;
        arg0->to.target[1] = 170.0f;
        arg0->to.target[2] = -113.0f;
        arg1->phase++;
        arg1->count = 0;
    case 1:
        t = arg1->count;
        arg1->count = t + 1;
        arg0->to.target[0] = t * -5.1041665f + 333.0f;
        arg0->to.target[2] = t * 4.3125f - 113.0f;
        if (arg1->count >= 0x60) {
            arg1->count = 0;
            arg1->phase++;
        }
        break;
    case 2:
        if (arg1->count >= 24) {
            arg0->to.target[0] -= -3.925f;
            arg0->to.target[2] -= 7.525f;
        }
        f = VuCosAngle(arg1->count << 9);
        arg0->to.dist += 6.25f;
        arg0->to.rot[1] = -(f * 17280.0f) + 23680.0f;
        arg0->to.rot[0] = arg0->to.rot[0] - 22.0f;
        arg1->count++;
        if (arg1->count >= 0x40) {
            BtlCamSetMode(1, 0x40, 0, NULL);
        }
        break;
    }
}
