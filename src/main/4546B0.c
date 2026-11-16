#include "common.h"
#include "main/n08.h"

void McReset(void) {
    s32 i;
    McReq *req;

    mcReqQueue.count = 0x20;
    for (i = 0; i < mcReqQueue.count; i++) {
        mcReqQueue.slots[i] = NULL;
    }
    mcReqPool.count = 0x10;
    for (i = 0; i < mcReqPool.count; i++) {
        mcReqPool.slots[i].req.id = i;
        mcReqPool.slots[i].used = 0;
        req = &mcReqPool.slots[i].req;
        req->fd = 0;
        req->arg[0] = 0;
        req->arg[1] = 0;
        req->arg[2] = 0;
        req->cmd = 0;
    }
}
