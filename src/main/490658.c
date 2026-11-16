/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"



s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 SignalSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceRead(s32 fd, void *buf, s32 len) {
    SemaParam sp;
    s32 result;
    SceFsFd *f;
    SceFsReadReq *p;
    s32 flags;
    s32 mode;
    s32 sema;
    s32 ok;
    s32 i;

    p = &_sceFsSendBuf.read;
    f = _sceFsGetFd(fd);
    _sceFsLock(2);
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
    sp.maxCount = 1;
    p->fdIndex = f - _sceFsFdTable;
    p->buf = buf;
    p->len = len;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    p->result = &result;
    p->resultSize = 4;
    _sceFsSendBuf.read.sema = sema;
    if (flags & 0x8000) {
        WaitSema(_sceFsAsyncSema);
        i = 0;
        while (i < 32) {
            if (_sceFsAsyncSemaTable[i] == -1) {
                _sceFsAsyncSemaTable[i] = p->sema;
                p->sema = -p->sema;
                break;
            }
            i++;
        }
        SignalSema(_sceFsAsyncSema);
    }
    if (!(flags & 0x20000000)) {
        sceSifWriteBackDCache(buf, len);
    }
    sceSifWriteBackDCache(&_sceFsRetBuf, 0xA4);
    sceSifWriteBackDCache(p, 0x20);
    if (sceSifCallRpc(&_sceFsClient, 2, 0, &_sceFsSendBuf, 0x20, _sceFsRecvBuf, 4, 0, 0) < 0) {
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
