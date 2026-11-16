#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"


void BtlStartCamDefaultZoom(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    f32 t;

    switch (arg1->phase) {
    case 0:
        arg0->to.rot[0] = 0x31C7;
        arg0->to.rot[1] = 0x8000;
        arg0->to.rot[2] = 0;
        arg1->count = -0x48;
        arg0->to.target[0] = 0.0f;
        arg0->to.target[1] = 0.0f;
        arg1->phase++;
        arg0->to.target[2] = 0.0f;
    case 1:
        arg1->count++;
        t = arg1->count * 0.25f;
        arg0->to.dist = t * t * 1.5f + 470.0f;
        if (arg1->count >= 0x58) {
            BtlCamSetMode(1, 0x40, 0, NULL);
        }
        break;
    }
}
