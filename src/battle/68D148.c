#include "common.h"
#include "battle/misc.h"

void FxTrailSetWidth(s32 i, f32 a, f32 b) {
    FxTrail *e = &fxTrails[i];
    if (i >= 0 && e->handle != 0) {
        e->unk28 = a;
        e->unk2C = b;
    }
}
