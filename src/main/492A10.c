/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceSymlink(u8 *str1, u8 *str2) {
    SemaParam sema;
    s32 result;
    SceFsPath2Req *req;
    s32 sid;
    s32 r;
    s32 i;

    req = &_sceFsSendBuf.path2;
    _sceFsLock(0x11);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    for (i = 0; i < 0x400; i++) {
        if ((req->name1[i] = str1[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        req->name1[0x3FF] = 0;
    }
    for (i = 0; i < 0x400; i++) {
        if ((req->name2[i] = str2[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        req->name2[0x3FF] = 0;
    }
    sema.initCount = 0;
    sema.maxCount = 1;
    sema.option = 0;
    sid = CreateSema(&sema);
    req->result = &result;
    req->sema = sid;
    req->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 0x18, 0, &_sceFsSendBuf, 0x80C, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
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
    WaitSema(sid);
    DeleteSema(sid);
    return result;
}
