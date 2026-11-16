#include "common.h"
#include "protos/main.h"
#include "party.h"





void PtyCharGainExp(PtyUnit *arg0) {
    s32 idx;
    s32 lv;
    s32 next;
    s32 *tbl;
    s32 l0;
    s32 *p;
    s32 *q;

    idx = arg0->id - 0x1001;
    if (idx < 0) {
        return;
    }
    l0 = arg0->level;
    if (l0 >= 250) {
        return;
    }
    tbl = ptyGrowthTable[idx].exp;
    lv = l0;
    p = &tbl[lv];
    if (arg0->exp >= *p) {
        do {
            next = lv + 1;
            PtyCharLevelUpStats(idx, next, arg0);
            lv = next;
            if (next >= 250) {
                break;
            }
            q = &tbl[lv];
        } while (arg0->exp >= *q);
    }
    PtyCharUpdateLevel(arg0);
    PtyUnitCalcParams(arg0, 0, 0);
}
