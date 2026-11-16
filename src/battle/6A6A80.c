#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"


void BtlStartCamBossLongPan(BtlCamCtrl *arg0, BtlCamParam *arg1) {
    switch (arg1->phase) {
    case 0:
        BtlCamSetCalcMode(2);
        arg1->count = 0;
        arg1->phase++;
        btlStartCamPanDist = arg0->from.eye[1] - arg0->from.target[1] + 300.0f;
    case 1:
        arg1->count++;
        arg0->to.eye[1] = btlStartCamPanDist * (1.0f - arg1->count * (1.0f / 180.0f)) + arg0->to.target[1] - 300.0f;
        if (arg1->count >= 180) {
            arg1->count = 0;
            arg1->phase++;
        }
        break;
    case 2:
        arg1->count++;
        if (arg1->count >= 120) {
            BtlCamSetMode(1, 0x40, 0, NULL);
        }
        break;
    }
}
