#include "common.h"
#include "party.h"
#include "battle/unit.h"

s8 BtlUnitCountManaEggs(BtlUnit *arg0) {
    PtyUnit *s;
    s32 i;
    s8 n;
    s32 v;

    s = arg0->params;
    i = ptyCharDefTable[s->id - 0x1001].eggSlots - 1;
    n = 0;
    for (; i >= 0; i--) {
        v = ptyCharData[arg0->params->id - 0x1001].slots[i];
        if (v >= 0 && v < 0x18) {
            n++;
        }
    }
    return n;
}
