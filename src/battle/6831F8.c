#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/n28.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))


void BtlSpecialIntroCamMember3(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.dist = 1200.0f;
        BtlChrModelInvalidate(x->unitPtr->id);
        BtlChrGetPartPos(&c->to.target, x->unitPtr, 1);
        c->to.rot[0] = 0x2800;
        c->to.rot[2] = 0;
        c->to.target[1] += 10.0f;
        if (btlSpecialIntro.count == 4) {
            c->to.target[0] = 20.0f;
            c->to.target[2] = 180.0f;
            c->to.rot[1] = 0x10000;
        } else {
            c->to.rot[1] = x->unitPtr->rot[1] + 0x8000;
        }
        x->phase++;
    }
}
