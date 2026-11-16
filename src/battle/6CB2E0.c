#include "common.h"
#include "battle/camera.h"

void BtlCamGameOverCb(BtlCamCtrl *c) {
    s32 pitch;

    pitch = c->to.rot[0];
    c->to.dist += 0.25f;
    if (pitch >= 0x1001) {
        c->to.rot[0] = pitch - 4;
    }
    c->to.rot[1] += 8;
}
