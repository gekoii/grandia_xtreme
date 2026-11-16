#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "party.h"


void BtlStartCamSpecial(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    BtlUnit *c;
    s32 a;
    s32 t;

    switch (arg1->phase) {
    case 0:
        arg0->to.dist = 250.0f;
        c = btlEnemyUnits;
        BtlChrGetPartPos(arg0->to.target, c, 1);
        arg0->to.rot[0] = 0x3E00;
        arg0->to.target[1] = (arg0->to.target[1] + c->params->colH) * 0.5f;
        a = c->rot[1];
        if (((a + 0x8000) & 0xFFFF) > 0x8000) {
            t = (u16)(a + 0x7800);
        } else {
            t = (a - 0x7800) & 0xFFFF;
        }
        arg0->to.rot[1] = t;
        arg0->to.rot[2] = 0;
        arg1->count = 0;
        arg1->phase++;
    case 1:
        arg0->to.dist += 7.8125f;
        if (arg1->count >= 0x60) {
            arg0->to.rot[0] = VuCosAngle((arg1->count - 0x60) << 9) * 3584.0f + 12288.0f;
        }
        arg1->count++;
        if (arg1->count >= 0x80) {
            BtlCamSetMode(1, 0x40, 0, NULL);
        }
        break;
    }
}
