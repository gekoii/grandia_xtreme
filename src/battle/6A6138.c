#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"


void BtlStartCamDefault(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    if (arg1->phase != 0) {
        BtlCamSetMode(0, 0xA0, 0, BtlStartCamDefaultZoom);
        return;
    }
    arg0->to.rot[0] = 0x4000;
    arg0->to.rot[1] = 1;
    arg0->to.rot[2] = 0;
    arg0->to.dist = 1280.0f;
    arg1->phase++;
    arg0->to.target[1] = 40.0f;
    arg0->to.target[2] = 0;
    arg0->to.target[0] = 0;
}
