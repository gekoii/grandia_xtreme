/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitStateHitGuard(BtlUnit *arg0) {
    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            BtlChrSetMotion(arg0, 8);
            arg0->evtLoop = 0;
            arg0->loopStart = arg0->loopEnd;
            arg0->subPhase++;
            arg0->stateCount = 0;
        case 1:
            if (arg0->stateCount >= 10) {
                arg0->unkD0[0] = arg0->unkD0[1] = arg0->unkD0[2] = 0.0f; arg0->subPhase++;
            } else {
                BtlUnitShakeOffset(arg0, 10.0f - arg0->stateCount);
            }
            break;
        case 2:
            if (arg0->frame == arg0->loopStart) {
                BtlUnitStateResume(arg0);
            }
            break;
        }
    }
}
