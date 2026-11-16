#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void FxTrailSetColorKind(s32 i, s32 k, s8 flag) {
    FxTrail *e = &fxTrails[i];
    if (i >= 0 && e->handle != 0 && e->unk21 < 0 && k >= 0 && k < 11) {
        if (flag != 0 && k > 0) {
            e->unk21 = FxTraceSparkAlloc(k);
        }
        e->c0.w = fxTraceColors[k].start.w;
        e->c1.w = fxTraceColors[k].end.w;
        e->c0.c.r >>= 1;
        e->c1.c.r >>= 1;
    }
}
