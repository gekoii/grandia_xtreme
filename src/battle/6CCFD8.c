/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitStateHitBlocked(BtlUnit *arg0) {
    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            if (arg0->id < 4) {
                BtlChrSetMotion(arg0, 7);
                arg0->frameSpeed = 2.0f;
            } else {
                BtlChrSetMotion(arg0, 0);
            }
            arg0->evtLoop = 0;
            arg0->loopStart = arg0->loopEnd;
            arg0->subPhase++;
            arg0->evtCur = 0;
            arg0->stateCount = 0;
        case 1:
            BtlUnitShakeOffset(arg0, 5.0f);
            if (arg0->stateCount >= 7) {
                arg0->stateCount = -1; arg0->subPhase++; arg0->unkD0[0] = arg0->unkD0[1] = arg0->unkD0[2] = 0.0f;
            }
            break;
        case 2:
            if (arg0->frame == arg0->loopStart || arg0->stateCount >= 50) {
                arg0->frameSpeed = 0.0f;
                arg0->frame = arg0->loopStart;
                BtlUnitSetState(arg0, 0x40);
            }
            break;
        }
    }
}
