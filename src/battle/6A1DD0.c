#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void FxWallFree(FxWall *arg0) {
    if (arg0->pool != 0) {
        MemFree(arg0->pool);
        arg0->pool = 0;
    }
    arg0->state = 0;
}
