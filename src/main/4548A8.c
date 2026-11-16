#include "common.h"
#include "main/n08.h"

s32 McReqFreeId(s32 arg0) {
    McReqSlot *e;

    if (arg0 < 0 || mcReqPool.count < arg0) {
        return 0;
    }
    e = &mcReqPool.slots[arg0];
    if (e->used == 0) {
        return 0;
    }
    e->used = 0;
    return 0;
}
