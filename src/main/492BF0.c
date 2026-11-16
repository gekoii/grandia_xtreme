/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


extern s32 CreateSema(SemaParam *);
extern s32 DeleteSema(s32);
extern s32 WaitSema(s32);
extern s32 sceSifWriteBackDCache(s32, u32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceReadlink(char *arg0, s32 arg1, u32 arg2) {
    SemaParam sp;
    s32 ret;
    s32 sema;
    s32 i;
    s32 ok;
    SceFsSyncReq *p;

    p = &_sceFsSendBuf.sync;
    _sceFsLock(0x11);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    for (i = 0; i < 0x400; i++) {
        if ((p->name[i] = arg0[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        p->name[0x3FF] = 0;
    }
    if (arg2 >= 0x400) {
        arg2 = 0x3FF;
    }
    p->arg = arg1;
    p->unkC = arg2;
    sceSifWriteBackDCache(arg1, arg2);
    sp.initCount = 0;
    sp.maxCount = 1;
    sp.option = 0;
    sema = CreateSema(&sp);
    p->result = &ret;
    p->sema = sema;
    p->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 0x19, 0, &_sceFsSendBuf, 0x80C, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsUnlock();
        return -11;
    }
    ok = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (ok == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return ret;
}
