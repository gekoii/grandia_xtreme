#include "common.h"
#include "battle/misc.h"

void FxTrailSetSparkScale(s32 i, f32 v) {
    FxTrail *e = &fxTrails[i];
    if (i >= 0 && e->handle != 0) {
        e->unk24 = v;
    }
}
