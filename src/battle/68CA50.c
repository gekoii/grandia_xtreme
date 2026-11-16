/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"

void FxTraceSetFadeOut(s32 i, s32 n) {
    FxTraceUnit *o = &fxTraceUnits[i];
    if (n > 0) {
        o->t = 1.0f / n;
    }
}
