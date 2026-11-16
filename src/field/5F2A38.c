#include "common.h"
#include "party.h"

s16 PtyCharCalcLevelB(s16 id) {
    s32 i;
    PtyGrowth *l = &ptyGrowthTable[id];

    if (ptyCharData[id].exp >= l->exp[0xF9]) {
        return 0xFA;
    }
    i = 1;
    if (ptyCharData[id].exp >= l->exp[1]) {
        do {
            i++;
        } while (ptyCharData[id].exp >= l->exp[i]);
    }
    return i;
}
