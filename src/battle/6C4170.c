/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))

s32 AiComboCanPerform(BtlUnit *u, s16 id, f32 lim) {
    PtyMove *t;
    BtlUnit *p;
    PtyUnit *st;
    s32 found;
    s32 cost;
    u32 lv;
    s32 i;
    s32 j;

    t = &ptyMoveTable[id & 0xFFF];
    found = 0;
    if (id >= 0x1000) {
        cost = 0;
    } else {
        lv = ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(id)].level;
        if (lv == 0) {
            lv = 1;
        } else if (ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(id)].maxLevel < lv) {
            lv = ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(id)].maxLevel;
        }
        cost = CEIL(t->levels[lv - 1].cost * u->params->spcSpRate);
    }
    for (j = 0; j < 4; j++) {
        if (t->members[j] > 0) {
            p = btlUnits;
            for (i = 0; i < 4; i++, p++) {
                if (p->state < 5) {
                    continue;
                }
                st = p->params;
                if (st->id - 0x1000 != t->members[j]) {
                    continue;
                }
                if (p->state < 0x40 || st->sp < cost || p->comboLeader >= 0 || p->comboPartners[0] >= 0 || (st->status & 0x23E0) || (p->flags & 0x1000)) {
                    return 0;
                }
                if (p == u) {
                    found = 1;
                } else if (p->ipMode == 2 || p->ipPos <= 0.0f || lim < p->ipPos) {
                    return 0;
                }
                break;
            }
            if (i >= 4) {
                return 0;
            }
        }
    }
    return found;
}
