#include "common.h"
#include "battle/camera.h"

void BtlSpecialIntroCamMember3Pose(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.rot[0] = 0x3C00;
        c->to.dist = 300.0f;
        x->phase = x->phase + 1;
    }
}
