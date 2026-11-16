#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

s8 McReqGetInfo(s32 arg0, s32 arg1, s32 arg2) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->arg[1] = arg1;
    req->arg[2] = arg2;
    req->status = 0;
    req->cmd = MC_CMD_GETINFO;
    McQueuePush(req);
    return req->id;
}

s8 McReqOpen(void *arg0, s32 arg1) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = (s32)arg0;
    req->arg[1] = arg1;
    req->status = 0;
    req->cmd = MC_CMD_OPEN;
    McQueuePush(req);
    return req->id;
}

s8 McReqRead(s32 arg0, s32 arg1, s32 arg2) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->arg[1] = arg1;
    req->arg[2] = arg2;
    req->status = 0;
    req->cmd = MC_CMD_READ;
    McQueuePush(req);
    return req->id;
}

s8 McReqWrite(s32 arg0, s32 arg1, s32 arg2) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->arg[1] = arg1;
    req->arg[2] = arg2;
    req->status = 0;
    req->cmd = MC_CMD_WRITE;
    McQueuePush(req);
    return req->id;
}

s8 McReqClose(s32 arg0) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->status = 0;
    req->cmd = MC_CMD_CLOSE;
    McQueuePush(req);
    return req->id;
}

s8 McReqDelete(s32 arg0) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->status = 0;
    req->cmd = MC_CMD_DELETE;
    McQueuePush(req);
    return req->id;
}

s32 McReqMkdir(s32 arg0) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->status = 0;
    req->cmd = MC_CMD_MKDIR;
    McQueuePush(req);
    return req->id;
}

s8 McReqChdir(s32 arg0, s32 arg1) {
    McReq *req;

    req = McReqAlloc();
    req->arg[0] = arg0;
    req->arg[1] = arg1;
    req->status = 0;
    req->cmd = MC_CMD_CHDIR;
    McQueuePush(req);
    return req->id;
}
