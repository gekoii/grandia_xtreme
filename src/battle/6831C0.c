#include "common.h"
#include "battle/camera.h"

void BtlSpecialIntroCamRagePull(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        x->phase = 1;
        c->to.dist = 1000.0f;
    }
    c->to.dist = c->to.dist + 1.0f;
}
