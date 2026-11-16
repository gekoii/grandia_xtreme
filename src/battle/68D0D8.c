#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"


void FxTrailFree(s32 i) {
    FxTrail *e = &fxTrails[i];
    if (i >= 0 && e->handle != 0) {
        s32 k = e->unk21;
        if (k >= 0) {
            fxTraceSparks[k].state = 1;
        }
        MemFree(e->handle);
        e->handle = 0;
    }
}
