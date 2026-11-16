/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 sceFormat(char *s1, char *s2, u8 *data, s32 len) {
    SemaParam sp;
    s32 result;
    SceFsMountReq *r;
    s32 i;
    s32 sema;
    s32 v;

    r = &_sceFsSendBuf.mount;
    _sceFsLock(14);
    if (_sceFsInitDone == 0) {
        sceFsInit();
    }
    for (i = 0; i < 0x400 && (r->name1[i] = s1[i]) != 0; i++);
    if (i == 0x400) {
        r->name1[0x3FF] = 0;
    }
    if (s2 == NULL) {
        r->name2[0] = 0;
    } else {
        for (i = 0; i < 0x400 && (r->name2[i] = s2[i]) != 0; i++);
        if (i == 0x400) {
            r->name2[0x3FF] = 0;
        }
    }
    if (len > 0x400) {
        _sceFsUnlock();
        return -7;
    }
    for (i = 0; i < len; i++) {
        r->arg[i] = data[i];
    }
    r->flag = len;
    sp.maxCount = 1;
    sp.initCount = 0;
    sp.option = 0;
    sema = CreateSema(&sp);
    r->result = &result;
    r->sema = sema;
    r->resultSize = 4;
    sceSifWriteBackDCache(&_sceFsSendBuf, 0xC10);
    if (sceSifCallRpc(&_sceFsClient, 14, 0, &_sceFsSendBuf, 0xC10, _sceFsRecvBuf, 4, 0, 0) < 0) {
        DeleteSema(sema);
        _sceFsUnlock();
        return -11;
    }
    v = *(s32 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsUnlock();
    if (v == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}
