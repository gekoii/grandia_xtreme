#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/n28.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))


void BtlSpecialIntroCamFace(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.dist = RND01() * 300.0f + 800.0f;
        BtlChrPartToWorld(&c->to.target, x->unitPtr, BtlChrFindPart(x->unitPtr->model, 0x28A), 0);
        c->to.rot[0] = RND01() * 15360.0f + 3072.0f;
        c->to.rot[1] = c->to.rot[1] + (RND01() * 32768.0f + 16384.0f);
        c->to.rot[2] = RND01() * 8192.0f + 61440.0f;
        x->phase++;
    }
}
