#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n32.h"

void BtlTargetFilterByStatus(BtlTargetSel *s, s32 start, s32 n, u16 mask, s8 inv) {
    s8 tmp[16];
    BtlUnit *u = &btlUnits[start];
    s32 cnt = 0;
    s32 i;

    for (i = start; i < n; i++, u++) {
        if (s->flags[i] != 0 && (inv ? !(u->params->status & mask) : (u->params->status & mask))) {
            tmp[i] = 1;
            cnt++;
        } else {
            tmp[i] = 0;
        }
    }
    if (cnt != 0) {
        for (; start < n; start++) {
            s->flags[start] = tmp[start];
        }
    }
}
