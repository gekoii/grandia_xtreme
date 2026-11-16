#include "common.h"
#include "battle/misc.h"

void FxTraceSetGrow(s32 i, s32 j, s32 v) {
    FxTraceUnit *o = &fxTraceUnits[i];
    s32 k = o->strips[j];
    if (k >= 0) {
        fxTraceStrips[k].unk14 = v;
    }
}
