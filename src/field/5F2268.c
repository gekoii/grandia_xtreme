#include "common.h"
#include "party.h"

s16 PtyCharCalcLevel(s16 id) {
    s32 lv;
    u32 exp;
    lv = 250;
    exp = ptyCharData[id].exp;
    if (exp < ptyGrowthTable[id].exp[249]) {
        for (lv = 1; exp >= ptyGrowthTable[id].exp[lv]; lv++) ;
    }
    return lv;
}
