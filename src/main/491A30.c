/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"



s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceDread(s32 unused, s32 arg) {
    SemaParam sema;
    SceFsArgReq *req;
    s32 result;
    SceFsFd *ent;
    s32 sid;
    s32 r;
    s32 *recv;

    req = &_sceFsSendBuf.arg;
    ent = ((SceFsFd *(*)(void))_sceFsGetFd)();
    ((void (*)(s32))_sceFsLock)(0xB);
    if (_sceFsInitDone == 0) {
        _sceFsUnlock();
        return -1;
    }
    if (ent == NULL || ent->flags == 0) {
        _sceFsUnlock();
        return -9;
    }
    req->arg0 = ent->fd;
    req->arg1 = arg;
    sema.maxCount = 1;
    sema.initCount = 0;
    sema.option = 0;
    sid = CreateSema(&sema);
    req->sema = sid;
    recv = (s32 *)_sceFsRecvBuf;
    req->result = &result;
    req->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 0xB, 0, req, 0x20, recv, 4, NULL, NULL) < 0) {
        WaitSema(sid);
        _sceFsUnlock();
        return -0xB;
    }
    r = *(s32 *)((u32)recv | 0x20000000);
    _sceFsUnlock();
    if (r == 0) {
        DeleteSema(sid);
        return -0xB;
    }
    WaitSema(sid);
    DeleteSema(sid);
    return result;
}
