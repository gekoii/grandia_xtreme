#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"


void FxTraceUpdate(void) {
    FxTraceUnit *a;
    BtlUnit *b;
    s32 i;

    a = fxTraceUnits;
    b = btlUnits;
    for (i = 15; i >= 0; i--, a++, b++) {
        if (b->state >= 0x40 && a->motion != 0) {
            FxTraceUnitUpdate(a, b);
        }
    }
    FxTrailUpdate();
    FxTraceSparkUpdate();
}
