#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

s32 McReqGetDir(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->arg[1] = arg1;
    req->arg[2] = arg2;
    req->arg[3] = arg3;
    req->status = 0;
    req->cmd = MC_CMD_GETDIR;
    McQueuePush(req);
    return req->id;
}
