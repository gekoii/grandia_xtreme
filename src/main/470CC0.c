/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "protos/main.h"



extern void SignalSema(s32);
extern s32 PollSema(s32);
extern char *strncpy(char *, const char *, s32);
extern void sceSifWriteBackDCache(void *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcChdir(s32 port, s32 slot, const char *name, void *info) {
    sceSifClientData *cd;
    SceMcNameParam *arg;
    u8 *buf;
    s32 ret;

    if (PollSema(_sceMcSema) < 0) {
        return -0xC8;
    }
    cd = &_sceMcClient;
    if (cd->serve == 0) {
        SignalSema(_sceMcSema);
        return -0x64;
    }
    if (name == NULL || *name == 0) {
        SignalSema(_sceMcSema);
        return -0xD2;
    }
    buf = _sceMcCurDirBuf;
    arg = &_sceMcNameArg;
    arg->port = port;
    arg->table = buf;
    arg->slot = slot;
    strncpy(arg->name, name, 0x3FF);
    arg->name[0x3FF] = 0;
    sceSifWriteBackDCache(buf, 0x400);
    ret = sceSifCallRpc(cd, 0xC, 1, arg, 0x414, &_sceMcRecvBuf, 4, _sceMcChdirApdx, info);
    if (ret == 0) {
        _sceMcCmdNo = 0xC;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
