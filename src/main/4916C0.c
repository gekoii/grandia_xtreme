/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceAddDrv(s32 arg0) {
    SemaParam sp;
    s32 result;
    s32 sema;
    s32 ok;
    SceFsArgReq *r;

    r = &_sceFsSendBuf.arg;
    _sceFsLock(0xF);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    r->arg0 = arg0;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    r->sema = sema;
    r->result = &result;
    r->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 0xF, 0, r, 0x10, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsUnlock();
        return -1;
    }
    ok = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (ok == 0) {
        DeleteSema(sema);
        return -1;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
