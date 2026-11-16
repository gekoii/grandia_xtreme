#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n29.h"


void BtlCmdTryLearnCombo(BtlUnit *arg0) {
    PtyCombo *e = ptyComboTable;
    PtyMove *pt;
    s32 i;

    for (i = 0; i < 50; i++, e++) {
        if (e->moveId > 0 && ptyPartyData->moveLevels[i + 40].level == 0 &&
            BtlCmdComboGetMembers(arg0, e, 1, 1) != 0) {
            pt = &ptyMoveTable[e->moveId];
            if (BtlCmdHasTarget(arg0, pt->kind) == 0) {
                return;
            }
            arg0->skillId = e->moveId;
            /* stored as a u32 bitfield (store order) */
            ((BtlUnitSkillLevelBits *)&arg0->skillLevel)->v = 1;
            arg0->combo = 1;
            arg0->action = 4;
            arg0->scopeRadius = pt->levels->scope;
            BtlUnitSetTargetArea(arg0, pt->target, pt->unk1);
            arg0->ipPos = 0;
            BtlUnitFixTarget(arg0, pt->kind, 1);
            return;
        }
    }
}
