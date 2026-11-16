#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"


void FxTraceStripFree(s32 i) {
    FxTraceStrip *e;
    s32 k;
    if (i >= 0) {
        if (fxTraceStrips[i].buf != 0) {
            e = &fxTraceStrips[i];
            k = e->unk17;
            if (k >= 0) {
                fxTraceSparks[k].state = 1;
            }
            MemFree(e->buf);
            e->buf = 0;
        }
    }
}
