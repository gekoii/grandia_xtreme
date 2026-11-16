/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


extern s32 CreateSema(SemaParam *);
extern s32 DeleteSema(s32);
extern s32 WaitSema(s32);
extern s32 sceSifWriteBackDCache(void *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceDevctl(char *arg0, s32 arg1, u8 *arg2, u32 arg3, s32 arg4, u32 arg5) {
    SemaParam sp;
    s32 ret;
    s32 sema;
    s32 i;
    u32 j;
    s32 ok;
    SceFsDevctlReq *p;

    p = &_sceFsSendBuf.devctl;
    _sceFsLock(0x17);
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
    if (arg3 > 0x400 || arg5 > 0x400) {
        _sceFsUnlock();
        return -22;
    }
    for (j = 0; j < arg3; j++) {
        p->arg[j] = arg2[j];
    }
    p->argLen = arg3;
    p->cmd = arg1;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    p->bufLen = arg5;
    p->result = &ret;
    p->resultSize = 4;
    p->buf = arg4;
    p->sema = sema;
    sceSifWriteBackDCache(&_sceFsSendBuf, 0x81C);
    if (sceSifCallRpc(&_sceFsClient, 0x17, 0, &_sceFsSendBuf, 0x81C, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
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
