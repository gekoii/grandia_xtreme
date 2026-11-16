#include "common.h"
#include "battle/n33.h"
#include "battle/camera.h"
#include "protos/battle.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))

void BtlCamEquipCb(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        BtlUnit *u = x->unitPtr;
        c->to.target[0] = u->pos[0];
        c->to.target[1] = 80.0f;
        c->to.target[2] = u->pos[2];
        c->to.dist = 400.0f;
        c->to.rot[0] = 0x3C00;
        c->to.rot[2] = RND01() * 4096.0f + 63488.0f;
        x->phase++;
    }
}
