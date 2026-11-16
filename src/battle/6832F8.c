#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/n28.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))

f32 VuSinAngle(s32);

void BtlSpecialIntroCamMember2(BtlCamCtrl *c, BtlCamParam *x) {
    if (x->phase == 0) {
        c->to.dist = 500.0f;
        BtlChrModelInvalidate(x->unitPtr->id);
        BtlChrGetPartPos(&c->to.target, x->unitPtr, 1);
        c->to.rot[0] = 0x2800;
        c->to.rot[2] = 0x800;
        if (btlSpecialIntro.count == 3) {
            c->to.target[0] = VuSinAngle(0x7000) * 400.0f + 140.0f;
            c->to.target[2] = VuCosAngle(0x7000) * 400.0f + 50.0f;
            c->to.rot[1] = 0x9F000;
        } else {
            c->to.target[0] += VuSinAngle(x->unitPtr->rot[1]) * 400.0f;
            c->to.target[2] += VuCosAngle(x->unitPtr->rot[1]) * 400.0f;
            c->to.rot[1] = x->unitPtr->rot[1] + 0x9800;
        }
        x->phase++;
    }
}
