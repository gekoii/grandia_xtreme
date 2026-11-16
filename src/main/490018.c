/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"

typedef char *va_list;
#define va_start(ap, last) \
    (ap = ((va_list)__builtin_next_arg(last) - \
           (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))



s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 SignalSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceOpen(const char *name, s32 flags, ...) {
    SemaParam sema;
    s32 result;
    SceFsOpenReq *req;
    va_list ap;
    s32 mode;
    SceFsFd *ent;
    s32 i;
    s32 idx;
    s32 sid;
    s32 ret;

    req = &_sceFsSendBuf.open;
    va_start(ap, flags);
    _sceFsLock(0);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    if (_sceFsVersionMismatch() != 0) {
        _sceFsUnlock();
        return -0x10004;
    }
    ent = _sceFsAllocFd();
    if (ent == NULL) {
        _sceFsUnlock();
        return -0x13;
    }
    mode = *(s32 *)ap;
    for (i = 0; i < 0x400; i++) {
        if ((req->name[i] = name[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        req->name[0x3FF] = 0;
    }
    idx = ent - _sceFsFdTable;
    req->flags = flags & 0x6FFFFFFF;
    req->mode = mode;
    sema.maxCount = 1;
    req->fdIndex = idx;
    sema.initCount = 0;
    sema.option = 0;
    sid = CreateSema(&sema);
    req->result = &result;
    req->sema = sid;
    req->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 0, 0, &_sceFsSendBuf, 0x418, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
        DeleteSema(sid);
        _sceFsUnlock();
        return -0xB;
    }
    ret = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (ret == 0) {
        DeleteSema(sid);
        return -0xB;
    }
    WaitSema(sid);
    DeleteSema(sid);
    if (result < 0) {
        WaitSema(_sceFsFdSema);
        ent->flags = 0;
        SignalSema(_sceFsFdSema);
        ret = result;
        return ret;
    } else {
        ret = idx;
        WaitSema(_sceFsFdSema);
        ent->fd = result;
        ent->flags |= flags;
        SignalSema(_sceFsFdSema);
    }
    return ret;
}
