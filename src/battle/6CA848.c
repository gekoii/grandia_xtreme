#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/camera.h"


void BtlUnitStateEscapeFollow(BtlUnit *arg0) {
    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            BtlChrSetMotion(arg0, 3);
            arg0->stateCount = 0;
            arg0->moveTarget[0] = btlCamParam.eye[0];
            arg0->moveTarget[1] = 0.0f;
            arg0->subPhase++;
            arg0->moveTarget[2] = btlCamParam.eye[2];
        case 1:
            BtlUnitMoveToward(arg0, &arg0->moveTarget, 2, BtlUnitGetMoveSpeed(arg0), 10.0f);
            break;
        }
    }
}
