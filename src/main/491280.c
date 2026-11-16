/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMkdir(u8 *str, s32 arg) {
    SemaParam sema;
    s32 result;
    SceFsArgPathReq *req;
    s32 sid;
    s32 r;
    s32 i;

    req = &_sceFsSendBuf.argPath;
    _sceFsLock(7);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    for (i = 0; i < 0x400; i++) {
        if ((req->name[i] = str[i]) == 0) {
            break;
        }
    }
    if (i == 0x400) {
        req->name[0x3FF] = 0;
        i = 0x3FF;
    }
    req->arg = arg;
    sema.maxCount = 1;
    sema.initCount = 0;
    sema.option = 0;
    sid = CreateSema(&sema);
    req->result = &result;
    req->sema = sid;
    req->resultSize = 4;
    if (sceSifCallRpc(&_sceFsClient, 7, 0, &_sceFsSendBuf, i + 0x11, _sceFsRecvBuf, 4, NULL, NULL) < 0) {
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
