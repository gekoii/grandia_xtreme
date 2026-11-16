#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "protos/battle.h"

void BtlCmdComboLinkPartners(BtlUnit *arg0) {
    s8 n = 0;
    s32 fresh;
    s32 i, j;
    BtlUnit *u;
    PtyMove *pt = &ptyMoveTable[arg0->skillId];
    s32 k = BtlSpecialMoveRecordIndex(arg0->skillId);

    fresh = ptyPartyData->moveLevels[k].level == 0;
    arg0->flags |= 0x80;

    for (i = 0; i < 4; i++) {
        if (pt->members[i] > 0 && pt->members[i] != arg0->def->id - 0x1000) {
            u = btlUnits;
            for (j = 0; j < 4; j++, u++) {
                if (u->state >= 4 && pt->members[i] == u->def->id - 0x1000) {
                    arg0->comboPartners[n] = u->id;
                    u->comboLeader = arg0->id;
                    if (fresh) u->ipPos = 0;
                    n++;
                    break;
                }
            }
        }
    }
}
