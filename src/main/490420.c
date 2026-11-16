/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"



s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 SignalSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 sceLseek(s32 id, s32 a, s32 b) {
    SemaParam sp;
    s32 result;
    SceFsFd *e;
    s32 flags;
    s32 mode;
    s32 i;
    s32 v;
    s32 sema;
    SceFsLseekReq *r;

    r = &_sceFsSendBuf.lseek;
    e = _sceFsGetFd(id);
    _sceFsLock(4);
    if (_sceFsInitDone == 0) {
        _sceFsUnlock();
        return -1;
    }
    if (e == NULL || (flags = e->flags) == 0) {
        _sceFsUnlock();
        return -9;
    }
    mode = e->flags;
    r->fd = e->fd;
    r->offset = a;
    r->whence = b;
    r->fdIndex = e - _sceFsFdTable;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    r->result = &result;
    r->resultSize = 4;
    _sceFsSendBuf.lseek.sema = sema;
    if (flags & 0x8000) {
        WaitSema(_sceFsAsyncSema);
        for (i = 0; i < 32; i++) {
            if (_sceFsAsyncSemaTable[i] == -1) {
                _sceFsAsyncSemaTable[i] = r->sema;
                r->sema = -r->sema;
                break;
            }
        }
        SignalSema(_sceFsAsyncSema);
    }
    if (sceSifCallRpc(&_sceFsClient, 4, 0, &_sceFsSendBuf, 0x1C, _sceFsRecvBuf, 4, 0, 0) < 0) {
        DeleteSema(sema);
        _sceFsUnlock();
        return -11;
    }
    v = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (v == 0) {
        DeleteSema(sema);
        return -11;
    }
    if (mode & 0x8000) {
        DeleteSema(sema);
        return 0;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
