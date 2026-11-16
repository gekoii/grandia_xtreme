#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void FxWallShardsFree(FxWallShards *arg0) {
    if (arg0->pool != 0) {
        MemFree(arg0->pool);
        arg0->pool = 0;
    }
    if (arg0->shards != NULL) {
        MemFree((s32)arg0->shards);
        arg0->shards = NULL;
    }
    arg0->kind = 0;
}
