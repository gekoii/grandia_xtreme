/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void BtlUnitStateHitLight(BtlUnit *arg0) {
    PtyUnit *info;

    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            BtlChrSetMotion(arg0, 1);
            arg0->evtLoop = 0;
            arg0->loopStart = arg0->loopEnd;
            arg0->subPhase++;
            arg0->stateCount = 0;
        case 1:
            BtlUnitShakeOffset(arg0, 5.0f);
            if (arg0->stateCount >= 7) {
                arg0->subPhase++; arg0->unkD0[0] = arg0->unkD0[1] = arg0->unkD0[2] = 0.0f;
            }
            break;
        case 2:
        case 3:
            info = arg0->params;
            if (!(*(volatile s32 *)&info->bitCsf & 4)) { /* volatile read: bitCsf is loaded twice, as in the original */
                if (arg0->knockback < 0.0f) {
                    arg0->knockback = 0.0f;
                }
                BtlUnitMoveDir(arg0, arg0->rot[1] + 0x8000, (info->bitCsf & 0x200) ? 1 : 3, arg0->knockback);
                arg0->knockback -= 1.0f;
            }
            if (arg0->subPhase == 2) {
                if (arg0->frameSpeed <= 0.0f) {
                    if (arg0->id < 4) {
                        BtlUnitSetState(arg0, 0x40);
                    } else {
                        arg0->stateCount = 0;
                        arg0->subPhase = 3;
                    }
                }
            } else if (arg0->stateCount + arg0->loopEnd >= 68.0f) {
                BtlUnitSetState(arg0, 0x40);
            }
            break;
        }
    }
    if (BtlChrMotionGetEvent(arg0, 2, 0) != 0) {
        arg0->frameSpeed = 0.0f;
    }
}
