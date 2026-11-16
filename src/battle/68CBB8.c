#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"


void FxTraceSparkUpdate(void) {
    FxTraceSpark *e = fxTraceSparks;
    s32 i;
    for (i = 0; i < 16; i++, e++) {
        if (e->handle != 0) {
            if (e->state >= 2) {
                if (e->time != btlFrameCount) {
                    MemFree(e->handle);
                    e->handle = 0;
                }
            } else if (PtclBtlSprUpdate(e->handle, e->count, e->unk3) <= 0 && e->state == 1) {
                e->state = 2;
                e->time = btlFrameCount;
            }
        }
    }
}
