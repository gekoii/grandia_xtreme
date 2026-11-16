#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n29.h"

void BtlCmdTryLearnMove(BtlUnit *arg0) {
    s32 k = arg0->params->id - 0x1001;
    PtyMoveLevel *sl = &ptyPartyData->moveLevels[k * 5];
    PtyMoveLevelThreshold *r = &ptyMoveLevelThresholds[k * PTY_CHAR_MOVES];
    PtyMove *pt;
    s32 i, j;

    for (i = 0; i < 5; i++, sl++, r++) {
        if (sl->level != 0) {
            continue;
        }
        for (j = 0; j < 2; j++) {
            if (r->req[j] >= 0 && sl[r->req[j] - i].level < r->lvl[j]) {
                break;
            }
        }
        if (j >= 2) {
            break;
        }
    }
    if (i < 5) {
        pt = &ptyMoveTable[sl->moveId];
        if (BtlCmdHasTarget(arg0, pt->kind) != 0) {
            arg0->skillId = sl->moveId;
            /* stored as a u32 bitfield (store order) */
            ((BtlUnitSkillLevelBits *)&arg0->skillLevel)->v = 1;
            arg0->combo = 1;
            arg0->action = 4;
            arg0->scopeRadius = pt->levels->scope;
            BtlUnitSetTargetArea(arg0, pt->target, pt->unk1);
            arg0->ipPos = 0;
            BtlUnitFixTarget(arg0, pt->kind, 1);
        }
    }
}
