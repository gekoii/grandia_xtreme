/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "protos/battle.h"
#include "battle/n32.h"

s32 BtlTargetFilterByRate(BtlTargetSel *s, s32 start, s32 n) {
    s8 tmp[16];
    BtlUnit *u = &btlUnits[start];
    PtyUnit *st;
    s32 cnt = 0;
    s32 i;

    for (i = start; i < n; i++, u++) {
        if (s->flags[i] != 0 && (st = u->params, st->lure > 0.0f)
            && (rand() >> 15) * (1.0f / 65536.0f) <= st->lure) {
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
        return 1;
    }
    return 0;
}
