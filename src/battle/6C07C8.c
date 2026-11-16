#include "common.h"
#include "battle/unit.h"

s32 BtlUnitsNoScriptedReaction(void) {
    BtlUnit *u;
    s32 i;

    u = btlUnits;
    for (i = 0; i < 16; i++, u++) {
        if (u->state == 0x7F) {
            return 0;
        }
    }
    return 1;
}
