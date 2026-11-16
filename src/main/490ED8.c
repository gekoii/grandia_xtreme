/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"



void memcpy(void *, void *, u32);
s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceIoctl2(s32 arg0, s32 arg1, void *arg2, u32 arg3, s32 arg4, u32 arg5) {
    SemaParam sp;
    s32 result;
    SceFsFd *h;
    s32 sema;
    s32 ok;
    SceFsIoctlReq *r;

    r = &_sceFsSendBuf.ioctl;
    h = _sceFsGetFd(arg0);
    _sceFsLock(0x1A);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    if (h == NULL || h->flags == 0) {
        _sceFsUnlock();
        return -9;
    }
    if (arg3 > 0x400 || arg5 > 0x400) {
        _sceFsUnlock();
        return -0x16;
    }
    if (arg2 == NULL) {
        r->argLen = 0;
    } else {
        memcpy(r->arg, arg2, arg3);
    }
    r->fd = h->fd;
    r->cmd = arg1;
    r->argLen = arg3;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    r->bufLen = arg5;
    r->result = &result;
    r->resultSize = 4;
    r->buf = arg4;
    r->sema = sema;
    sceSifWriteBackDCache(&_sceFsSendBuf, 0x420);
    if (sceSifCallRpc(&_sceFsClient, 0x1A, 0, &_sceFsSendBuf, 0x420, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsUnlock();
        return -0xB;
    }
    ok = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (ok == 0) {
        DeleteSema(sema);
        return -0xB;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
