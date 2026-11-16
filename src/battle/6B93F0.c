#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/misc.h"


void BtlUnitsUpdate(void) {
    BtlUnit *u;
    s32 i;

    u = btlUnits;
    for (i = 15; i >= 0; ) {
        i--;
        if (u->state > 0) {
            BtlUnitUpdate(u);
            FxUnitUpdate(u);
        }
        u++;
        btlDbgStage += 0x100000;
        if (i < 0) break;
    }
}
