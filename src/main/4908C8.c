/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"



s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 SignalSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(u32, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceWrite(s32 fd, u32 addr, s32 len) {
    SemaParam sema;
    s32 result;
    SceFsWriteReq *req;
    SceFsFd *ent;
    s32 flags;
    s32 mode;
    s32 sid;
    s32 r;
    s32 i;
    s32 j;
    s32 n;
    s32 *recv;
    s32 off;
    s32 *p;

    req = &_sceFsSendBuf.write;
    ent = _sceFsGetFd(fd);
    _sceFsLock(3);
    if (_sceFsInitDone == 0) {
        _sceFsUnlock();
        return -1;
    }
    if (ent == NULL || (flags = ent->flags) == 0) {
        _sceFsUnlock();
        return -9;
    }
    mode = ent->flags;
    req->fd = ent->fd;
    sema.maxCount = 1;
    req->fdIndex = ent - _sceFsFdTable;
    req->len = len;
    req->addr = addr;
    sema.initCount = 0;
    sema.option = 0;
    sid = CreateSema(&sema);
    req->result = &result;
    req->resultSize = 4;
    req->sema = sid;
    if (flags & 0x8000) {
        WaitSema(_sceFsAsyncSema);
        /* The empty loop's notes are a scheduling barrier; the dead use of p
         * keeps the %hi of _sceFsAsyncSemaTable ahead of it until gcse. */
        p = (s32 *)_sceFsAsyncSemaTable;
        i = 0;
        do { } while (0);
        if (i) off = (s32)p;
        off = addr & 0xF;
        for (; i < 0x20; i++) {
            if (((s32 *)_sceFsAsyncSemaTable)[i] == -1) {
                ((s32 *)_sceFsAsyncSemaTable)[i] = req->sema;
                req->sema = -req->sema;
                break;
            }
        }
        SignalSema(_sceFsAsyncSema);
    } else {
        off = addr & 0xF;
    }
    if (off == 0) {
        n = 0;
    } else {
        { u32 t = addr - 0x10; n = ((addr >> 4) << 4) - t; }
    }
    if (len < n) {
        n = len;
    }
    if (!(flags & 0x20000000)) {
        sceSifWriteBackDCache(addr, len);
    }
    addr |= 0x20000000;
    req->headLen = n;
    for (j = 0; j < n; j++) {
        req->head[j] = ((u8 *)addr)[j];
    }
    if (sceSifCallRpc(&_sceFsClient, 3, 0, &_sceFsSendBuf, 0x30, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
        DeleteSema(sid);
        _sceFsUnlock();
        return -0xB;
    }
    r = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (r == 0) {
        DeleteSema(sid);
        return -0xB;
    }
    if (mode & 0x8000) {
        DeleteSema(sid);
        return 0;
    }
    WaitSema(sid);
    DeleteSema(sid);
    return result;
}
