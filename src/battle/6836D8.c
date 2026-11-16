#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/n28.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))


void BtlSpecialIntroCamLeader(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.dist = 50.0f;
        BtlChrModelInvalidate(x->unitPtr->id);
        BtlChrPartToWorld(&c->to.target, x->unitPtr, BtlChrFindPart(x->unitPtr->model, 0x28A), 0);
        c->to.rot[0] = 0x3C00;
        c->to.rot[1] = x->unitPtr->rot[1] + 0x8000;
        c->to.rot[2] = 0;
        x->phase++;
    }
}
