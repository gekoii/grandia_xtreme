#include "common.h"
#include "battle/camera.h"
#include "battle/n28.h"
#include "protos/battle.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))

void BtlSpecialIntroCamZoom(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.dist = RND01() * 150.0f + 250.0f;
        x->phase++;
    }
}
