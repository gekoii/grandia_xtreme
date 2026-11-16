/* as: ee-gcc2.9-991111-01 */
/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/n31.h"
#include "party.h"


void BtlStartCamBoss(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    BtlUnit *c;
    s32 a;
    s32 t;
    f32 f;
    f32 g;

    switch (arg1->phase) {
    case 0:
        c = btlUnits;
        arg0->ofs.dist = c[4].params->colR * 1.5f + 150.0f;
        BtlChrGetPartPos(arg0->to.target, &c[4], 1);
        arg0->to.target[1] = (arg0->to.target[1] + c[4].params->colH) * 0.5f;
        if (arg0->to.target[1] < arg0->ofs.dist) {
            arg0->ofs.rot[0] = BtlAsinAngle(arg0->to.target[1] / arg0->ofs.dist);
        } else {
            arg0->ofs.rot[0] = 0x3000;
        }
        a = btlEnemyUnits[0].rot[1];
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
        f = VuCosAngle(arg1->count * 128.0f);
        arg0->to.rot[0] = arg0->ofs.rot[0] * f + 15360.0f;
        g = 1.0f - f;
        arg0->to.dist = arg0->ofs.dist + g * 1000.0f * g;
        arg1->count++;
        if (arg1->count >= 0x80) {
            BtlCamSetMode(1, 0x40, 0, NULL);
        }
        break;
    }
}
