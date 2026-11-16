/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"



s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 SignalSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s64 sceLseek64(s32 fd, s64 offset, s32 whence) {
    SemaParam sp;
    s64 result;
    SceFsFd *f;
    SceFsLseek64Req *p;
    s32 flags;
    s32 mode;
    s32 sema;
    s32 ok;
    s32 i;

    p = &_sceFsSendBuf.lseek64;
    f = _sceFsGetFd(fd);
    _sceFsLock(0x16);
    if (_sceFsInitDone == 0) {
        _sceFsUnlock();
        return -1;
    }
    if (f == NULL || (flags = f->flags) == 0) {
        _sceFsUnlock();
        return -9;
    }
    mode = f->flags;
    p->fd = f->fd;
    p->offset = offset;
    p->whence = whence;
    p->fdIndex = f - _sceFsFdTable;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    p->result = &result;
    p->resultSize = 8;
    _sceFsSendBuf.lseek64.sema = sema;
    if (flags & 0x8000) {
        WaitSema(_sceFsAsyncSema);
        for (i = 0; i < 32; i++) {
            if (_sceFsAsyncSemaTable[i] == -1) {
                _sceFsAsyncSemaTable[i] = p->sema;
                p->sema = -p->sema;
                break;
            }
        }
        SignalSema(_sceFsAsyncSema);
    }
    if (sceSifCallRpc(&_sceFsClient, 0x16, 0, &_sceFsSendBuf, 0x20, _sceFsRecvBuf, 4, 0, 0) < 0) {
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
    if (mode & 0x8000) {
        DeleteSema(sema);
        return 0;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
