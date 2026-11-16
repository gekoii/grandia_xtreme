/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceRename(char *arg0, char *arg1) {
    SemaParam sp;
    s32 result;
    s32 i;
    s32 sema;
    s32 ok;
    SceFsPath2Req *r;

    r = &_sceFsSendBuf.path2;
    _sceFsLock(0x11);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    for (i = 0; i < 0x400; i++) {
        if ((r->name1[i] = arg0[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        r->name1[0x3FF] = 0;
    }
    for (i = 0; i < 0x400; i++) {
        if ((r->name2[i] = arg1[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        r->name2[0x3FF] = 0;
    }
    sp.initCount = 0;
    sp.maxCount = 1;
    sp.option = 0;
    sema = CreateSema(&sp);
    r->result = &result;
    r->sema = sema;
    r->resultSize = 4;
    sceSifWriteBackDCache(&_sceFsSendBuf, 0x80C);
    if (sceSifCallRpc(&_sceFsClient, 0x11, 0, &_sceFsSendBuf, 0x80C, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
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
