#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

s32 FxTraceSparkAlloc(u32 n) {
    FxTraceSpark *e = fxTraceSparks;
    PtclBurstType *d;
    s32 i;
    s32 h;
    for (i = 0; i < 16; i++, e++) {
        if (e->handle == 0) {
            break;
        }
    }
    if (i >= 16 || n >= 17) {
        return -1;
    }
    d = &ptclBurstTypeTable[n];
    e->count = d->unk1;
    h = BtlAlloc32(e->count << 6);
    e->handle = h;
    if (h == 0) {
        return -1;
    }
    e->unk3 = n;
    e->unk2 = d->tex;
    e->frame = 0;
    e->state = 0;
    PtclBtlSprReset(h, e->count);
    e->unk5 = 0;
    return i;
}
