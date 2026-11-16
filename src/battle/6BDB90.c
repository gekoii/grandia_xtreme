/* cflags: -fno-gcse */
/* -fno-gcse: the s8 read stays an lb in each branch (gcse merges them into lbu + sign extension) */
#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n32.h"

void BtlTargetPickByRank(BtlTargetSel *s, s32 i, s32 n, s32 max) {
    BtlUnit *u;
    PtyUnit *st;
    s32 best;

    best = -1;
    u = &btlUnits[i];
    if (max == 0) {
        best = 99999;
    }
    for (; i < n; i++, u++) {
        if (s->flags[i] != 0) {
            st = u->params;
            if (max == 0 ? st->size < best : best < st->size) {
                best = st->size;
                s->idx = i;
            }
        }
    }
}
