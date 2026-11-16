/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitStateVanish(BtlUnit *arg0) {
    s32 t;
    s32 i;

    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            arg0->stateCount = 0;
            arg0->subPhase++;
        case 1:
            BtlUnitShakeOffset(arg0, (rand() >> 15) * (1.0f / 65536.0f) * 8.0f + 8.0f);
            if (arg0->stateCount >= 33) {
                arg0->stateCount = 0;
                arg0->subPhase++;
            }
            break;
        case 2:
            BtlUnitShakeOffset(arg0, (rand() >> 15) * (1.0f / 65536.0f) * 8.0f + 8.0f);
            if (arg0->stateCount >= 7) {
                arg0->stateCount = 0; arg0->unkD0[0] = arg0->unkD0[1] = arg0->unkD0[2] = 0.0f; arg0->subPhase++;
            }
            break;
        case 3:
            t = arg0->stateCount;
            arg0->pos[1] += t * 0.5f;
            arg0->scale[1] += t * 0.03125f;
            if (t >= 32) {
                arg0->alpha = 0.0f;
                if (arg0->id < 4 && btlPartyAliveCount <= 0) {
                    for (i = 0; i < 4; i++) {
                        if (btlUnits[i].state == 2) {
                            break;
                        }
                    }
                    if (i >= 4) {
                        BtlUnitSetState(arg0, 2);
                        return;
                    }
                }
                arg0->stateCount = 0;
                arg0->subPhase++;
                BtlSchedKoDone(arg0->id);
            } else {
                arg0->alpha = (32 - t) * 0.03125f;
            }
            break;
        case 4:
            if (arg0->stateCount >= 5) {
                if (btlPartyAliveCount > 0 && btlEnemyAliveCount > 0 && btlActingUnit == 0xFF) {
                    BtlCamSetMode(1, 0x80, 0, 0);
                }
                BtlUnitSetState(arg0, 0);
            }
            break;
        }
        if (BtlChrMotionGetEvent(arg0, 2, 0) != 0) {
            arg0->frameSpeed = 0.0f;
        }
    }
}
