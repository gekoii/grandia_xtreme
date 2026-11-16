/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"


void BtlUnitStateDefend(BtlUnit *arg0) {
    f32 f;

    switch (arg0->phase) {
    case 0:
        if (arg0->id < 4) {
            arg0->ipMode = 1;
            f = arg0->params->restartGuard;
arg0->ipRet = f; arg0->ipPos = f; arg0->flags &= 0xFFFFFF7F;
            arg0->ipSpeed = 0.0f;
            arg0->scopeType = 0;
            if (arg0->target < 4 || btlUnits[arg0->target].state < 0x40) {
                BtlUnitSelectTarget(arg0, 1, 0x1003);
            }
        }
        if (arg0->params->guardHp > 0) {
            BtlHitQueueAddDamage(arg0, arg0->params->guardHp, 0.0f, arg0->rot[1], 0x100, 0);
        }
        if (arg0->params->guardSp > 0) {
            BtlHitQueueAddDamage(arg0, arg0->params->guardSp, 0.0f, arg0->rot[1], 0x180, 0);
        }
arg0->ipSpeed = arg0->params->params[4] * btlInvPartyAvgSpeed / (btlPartyAliveCount * 60.0f + 240.0f); arg0->subPhase = 0; arg0->phase++;
    case 1:
        switch (arg0->subPhase) {
        case 0:
            arg0->flags |= 4;
            if (arg0->motionNo == 8) {
                BtlChrSetMotion(arg0, 7);
                BtlChrMotionSetFrame(arg0, BtlChrMotionFindKey(arg0, 0x407, 0) - 1.0f);
            } else {
                BtlChrSetMotion(arg0, 7);
                arg0->evtLoop = 0;
                arg0->loopStart = arg0->loopEnd;
            }
            arg0->subPhase++;
        case 1:
            BtlUnitTurnToTarget(arg0);
            if (arg0->ipPos <= 0.0f || btlPhase != 0x80) {
                arg0->subPhase = 0;
                arg0->phase++;
            }
            break;
        }
        break;
    case 2:
        switch (arg0->subPhase) {
        case 0:
            arg0->flags |= 4;
            BtlChrSetMotion(arg0, 7);
            BtlChrMotionSetFrame(arg0, BtlChrMotionFindKey(arg0, 0x408, 0));
            f = arg0->motionLast;
arg0->loopCount = -1; arg0->loopEnd = f; arg0->loopStart = f; arg0->subPhase++;
            arg0->evtLoop = 0;
            arg0->evtCur = 0;
        case 1:
            if (arg0->frame == arg0->loopStart) {
                arg0->flags &= 0xFFFFFFFB;
                BtlUnitSetState(arg0, 0x80);
            }
            break;
        }
        break;
    }
}
