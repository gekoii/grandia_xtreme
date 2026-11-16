#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

s8 McReqFormat(void) {
    McReq *req;

    req = McReqAlloc();
    req->status = 0;
    req->cmd = MC_CMD_FORMAT;
    McQueuePush(req);
    return req->id;
}

s8 McReqGetEntSpace(s32 arg0) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->status = 0;
    req->cmd = MC_CMD_GETENTSPACE;
    McQueuePush(req);
    return req->id;
}

s8 McReqRename(s32 arg0, s32 arg1) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->arg[1] = arg1;
    req->status = 0;
    req->cmd = MC_CMD_RENAME;
    McQueuePush(req);
    return req->id;
}
