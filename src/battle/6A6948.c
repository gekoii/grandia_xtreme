#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"

f32 VuSinAngle(s32);

void BtlStartCamBossLong(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    BtlUnit *c = btlEnemyUnits;
    f32 t;

    switch (arg1->phase) {
    case 0:
        BtlChrGetPartPos(arg0->to.target, c, 1);
        arg0->to.rot[0] = 0x3000;
        arg0->to.rot[1] = c->rot[1] + 0x8000;
        arg0->to.rot[2] = 0;
        arg0->to.dist = 400.0f;
        arg0->to.target[1] += 50.0f;
        arg1->phase++;
        arg1->count = 0;
    case 1:
        arg1->count++;
        t = 1.0f - VuSinAngle(arg1->count * 0x36);
        arg0->to.dist = t * t * -700.0f + 1100.0f;
        if (arg1->count >= 300) {
            BtlCamSetMode(0, 0, 0, BtlStartCamBossLongPan);
        }
        break;
    }
}
