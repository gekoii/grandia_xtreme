#include "common.h"
#include "battle/misc.h"
#include "battle/n30.h"

void FxWallStart(s32 *arg0) {
    FxWall *p = &fxWall[*arg0];

    p->state = 1;
    p->stamp = btlFrameCount;
    if (p->pool == 0) {
        p->stamp = btlFrameCount + 1;
    }
    p->model = NULL;
    p->flash = 0;
}
