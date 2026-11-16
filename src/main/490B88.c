/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"

typedef struct {
    u8 b[0x400];
} Blk;


typedef struct {
    s32 unk0;
    s32 unk4;
} H;

s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 SignalSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceIoctl(s32 arg0, s32 arg1, void *arg2) {
    SemaParam sp;
    s32 result;
    H *h;
    s32 sema;
    s32 ok;
    s32 i;
    SceFsIoctlReq *r;
    s32 one;

    one = 1;
    r = &_sceFsSendBuf.ioctl;
    h = _sceFsGetFd(arg0);
    _sceFsLock(5);
    _sceFsIoctlArg = arg2;
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    if (h == NULL || h->unk4 == 0) {
        _sceFsUnlock();
        return -9;
    }
    r->buf = 0;
    r->bufLen = 0;
    if (arg1 == 2) {
        goto c2;
    }
    if (arg1 < 3) {
        if (arg1 == one) {
            goto c1;
        }
        goto def;
    }
    if (arg1 == 3) {
        goto c3;
    }
    goto def;
c1:
    WaitSema(_sceFsAsyncSema);
    for (i = 0; i < 0x20 && _sceFsAsyncSemaTable[i] == -1; ) {
        i++;
    }
    if (i == 0x20) {
        *_sceFsIoctlArg = 0;
    } else {
        *_sceFsIoctlArg = 1;
    }
    SignalSema(_sceFsAsyncSema);
    goto done;
c2:
    *(s32 *)arg2 = *(s32 *)((u32)_sceFsRetBufData | 0x20000000);
    goto done;
c3:
    *(s64 *)arg2 = *(s64 *)((u32)_sceFsRetBufData | 0x20000000);
done:
    _sceFsUnlock();
    return 0;
def:
    r->fd = h->unk0;
    r->cmd = arg1;
    if (arg2 == NULL) {
        r->argLen = 0;
    } else {
        r->argLen = 0x400;
        *(Blk *)r->arg = *(Blk *)arg2;
    }
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    r->result = &result;
    r->sema = sema;
    r->resultSize = 4;
    sceSifWriteBackDCache(&_sceFsSendBuf, 0x420);
    if (sceSifCallRpc(&_sceFsClient, 5, 0, &_sceFsSendBuf, 0x420, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
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
