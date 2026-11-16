#include "common.h"
#include "battle/misc.h"

void FxTrailSetAngle(s32 i, u16 a) {
    FxTrail *e = &fxTrails[i];
    if (i >= 0 && e->handle != 0) {
        e->unk1E = a;
    }
}
