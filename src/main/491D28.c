/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"

typedef struct {
    u8 b[0x40];
} Blk;


s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceChstat(u8 *arg0, Blk *arg1, s32 arg2) {
    SemaParam sp;
    s32 result;
    s32 i;
    s32 sema;
    s32 ok;
    SceFsChstatReq *r;

    r = &_sceFsSendBuf.chstat;
    _sceFsLock(0xD);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    for (i = 0; i < 0x400; i++) {
        if ((r->name[i] = arg0[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        r->name[0x3FF] = 0;
        i = 0x3FF;
    }
    *(Blk *)r->stat = *arg1;
    r->cbit = arg2;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    r->result = &result;
    r->sema = sema;
    r->resultSize = 4;
    sceSifWriteBackDCache(&_sceFsSendBuf, 0x450);
    if (sceSifCallRpc(&_sceFsClient, 0xD, 0, &_sceFsSendBuf, i + 0x51, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
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
