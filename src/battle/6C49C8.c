#include "common.h"
#include "battle/unit.h"

s32 AiTargetTakenByOther(BtlUnit *u, s32 i, s32 n) {
    BtlUnit *p;

    p = &btlUnits[i];
    for (; i < n; i++, p++) {
        if (p->state >= 0x40 && p->ipMode == 2 && p->target == u->target && p->id != u->id) {
            return 1;
        }
    }
    return 0;
}
