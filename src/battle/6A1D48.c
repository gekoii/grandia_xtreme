#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


void FxWallHit(BtlUnit *arg0) {
    FxWall *p = &fxWall[arg0->id];

    if (p->state > 0) {
        /* do-while(0): keeps both calls from becoming tail calls (the original jals and returns) */
        do {
            if (arg0->wall > 0) {
                p->flash = 1.5f;
                FxWallShardsStart(arg0, 1);
            } else {
                p->state = 3;
                p->stamp = btlFrameCount;
                p->model = NULL;
                FxWallShardsStart(arg0, 2);
            }
        } while (0);
    }
}
