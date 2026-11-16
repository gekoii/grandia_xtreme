/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"

s32 BtlCmdComboGetMembers(BtlUnit *self, PtyCombo *p, s8 arg2, s8 arg3) {
    u8 mask;
    s32 k;
    s32 j;
    BtlUnit *c;
    PtyMoveLevel *e;

    mask = 0;
    for (k = 0; k < 4; k++) {
        if (p->chars[k] < 0) {
            continue;
        }
        c = btlUnits;
        for (j = 0; j < 4; j++, c++) {
            if (p->chars[k] != c->params->id - 0x1001) {
                continue;
            }
            if (c->state < 0x40) {
                continue;
            }
            if (c->comboLeader >= 0) {
                continue;
            }
            if (c->params->status & 0x23E0) {
                continue;
            }
            if (c->flags & 0x1000) {
                continue;
            }
            e = &ptyPartyData->moveLevels[p->chars[k] * 5 + p->moves[k]];
            if (e->level == 0) {
                continue;
            }
            if (c == self) {
                if (arg3 != 0 && e->moveId != self->skillId) {
                    continue;
                }
            } else {
                if (arg2 != 0 && c->state != 0x80) {
                    continue;
                }
                if (c->ipMode == 2) {
                    continue;
                }
                if (c->ipPos <= 0.0f) {
                    continue;
                }
                if (p->ipNeed[k] * 0.0001f < c->ipPos) {
                    continue;
                }
            }
            mask |= 1 << j;
            break;
        }
        if (j >= 4) {
            return 0;
        }
    }
    return ((mask >> self->id) & 1) ? mask : 0;
}
