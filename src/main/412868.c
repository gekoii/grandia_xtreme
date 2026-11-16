#include "common.h"
#include "party.h"




void PtyCharUpdateLevel(PtyUnit *u) {
    PtyGrowth *g;
    s32 exp;
    s32 i;

    exp = u->exp;
    g = &ptyGrowthTable[u->id - 0x1001];
    if (exp >= g->exp[249]) {
        u->nextExp = 0;
        u->level = 250;
    } else {
        for (i = 1; exp >= g->exp[i]; i++) {
        }
        u->level = i;
        u->nextExp = g->exp[i] - exp;
    }
}
