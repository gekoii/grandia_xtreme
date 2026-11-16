/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceSync(char *name, s32 arg) {
    SemaParam sp;
    s32 result;
    s32 sema;
    s32 ok;
    s32 i;
    SceFsSyncReq *p;

    p = &_sceFsSendBuf.sync;
    _sceFsLock(0x13);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    for (i = 0; i < 0x400; i++) {
        if ((p->name[i] = name[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        p->name[0x3FF] = 0;
    }
    p->arg = arg;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    p->result = &result;
    p->sema = sema;
    p->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 0x13, 0, &_sceFsSendBuf, 0x414, _sceFsRecvBuf, 4, 0, 0) < 0) {
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
