#include "common.h"
#include "main/n08.h"

McReq *McReqAlloc(void) {
    s32 i;
    McReq *req;

    for (i = 0; i < mcReqPool.count; i++) {
        if (mcReqPool.slots[i].used == 0) {
            req = &mcReqPool.slots[i].req;
            mcReqPool.slots[i].used = 1;
            return req;
        }
    }
    return NULL;
}
