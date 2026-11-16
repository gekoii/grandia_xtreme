#include "common.h"
#include "battle/misc.h"
#include "battle/n30.h"

void FxWallShardsStart(BtlUnit *arg0, s8 arg1) {
    FxWallShards *p = &fxWallShards[arg0->id];

    p->state = 1;
    p->kind = arg1;
    p->stamp = btlFrameCount;
    if (p->pool == 0) {
        p->stamp = btlFrameCount + 1;
    }
}
