#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/n28.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))


void BtlSpecialIntroCamMember1Pose(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.dist = 300.0f;
        BtlChrModelInvalidate(x->unitPtr->id);
        BtlChrGetPartPos(&c->to.target, x->unitPtr, 1);
        c->to.rot[0] = 0x4600;
        c->to.rot[1] = x->unitPtr->rot[1] + 0x4200;
        c->to.target[1] += 10.0f;
        x->phase++;
    }
}
