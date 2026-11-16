/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"



extern s32 CreateSema(SemaParam *);
extern s32 DeleteSema(s32);
extern s32 WaitSema(s32);
extern s32 sceSifCallRpc(void *, s32, s32, SceFsCloseReq *, s32, void *, s32, s32, s32);

s32 sceDclose(s32 fd) {
    SemaParam param;
    s32 result;
    SceFsFd *st;
    s32 sema;
    s32 r;
    SceFsCloseReq *req;

    req = &_sceFsSendBuf.close;
    st = _sceFsGetFd(fd);
    _sceFsLock(10);
    if (_sceFsInitDone == 0) {
        _sceFsUnlock();
        return -1;
    }
    if (st != NULL) {
        if (st->flags != 0) {
            goto ok;
        }
    }
    _sceFsUnlock();
    return -9;
ok:
    req->fd = st->fd;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    _sceFsSendBuf.close.sema = sema;
    req->result = &result;
    req->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 10, 0, req, 0x14, _sceFsRecvBuf, 4, 0, 0) < 0) {
        DeleteSema(sema);
        _sceFsUnlock();
        return -11;
    }
    st->flags = 0;
    r = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (r == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    if (result < 0) return result;
    return 0;
}
