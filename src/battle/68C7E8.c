#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void FxTraceStopAll(s32 i) {
    FxTraceUnit *o = &fxTraceUnits[i];
    s32 j;
    if (o->motion != 0) {
        for (j = 0; j < 6; j++) {
            if (o->strips[j] >= 0) {
                o->flags[j] = 1;
                FxTraceStripFree(o->strips[j]);
                o->strips[j] = -1;
            }
        }
        o->motion = 0;
    }
}
