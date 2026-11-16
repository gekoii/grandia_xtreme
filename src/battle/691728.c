#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"

s32 BtlCmdListComboCheck(PtyMove *p, s32 arg1, s32 arg2) {
    s32 flags;
    s32 k;
    s32 j;
    BtlUnit *c;

    flags = 1;
    for (k = 0; k < 4; k++) {
        if (p->members[k] > 0) {
            c = btlUnits;
            if (p->members[k] == arg2) {
                arg2 = -1;
            }
            for (j = 0; j < 4; j++, c++) {
                if (c->state < 4) {
                    continue;
                }
                if (p->members[k] != c->def->id - 0x1000) {
                    continue;
                }
                if (c->state < 0x40 || c->comboLeader >= 0 || c->comboPartners[0] >= 0 ||
                    (c->params->status & 0x23E0) || (c->flags & 0x1000)) {
                    flags |= 2;
                    btlCmdListComboNg |= 1 << j;
                } else if (c->params->sp < arg1) {
                    /* written twice: one |= becomes a movn (jump.c cmov) */
                    flags |= 2;
                    flags |= 2;
                }
                break;
            }
            if (j >= 4) {
                return 0;
            }
        }
    }
    if (arg2 >= 0) {
        flags = 0;
    }
    return flags;
}
