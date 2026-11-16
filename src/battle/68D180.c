#include "common.h"
#include "battle/misc.h"

void FxTrailSetColors(s32 i, s32 a, s32 b) {
    FxTrail *e = &fxTrails[i];
    if (i >= 0 && e->handle != 0) {
        e->c0.w = a;
        e->c1.w = b;
    }
}
