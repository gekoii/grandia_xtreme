#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/n28.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))


void BtlSpecialIntroCamRage(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.dist = 500.0f;
        BtlChrGetPartPos(&c->to.target, x->unitPtr, 1);
        c->to.rot[0] = RND01() * 3072.0f + 13312.0f;
        c->to.rot[1] = x->unitPtr->rot[1] + 0x8000;
        c->to.rot[2] = 0;
        x->phase++;
    }
    c->to.dist -= 1.0f;
}
