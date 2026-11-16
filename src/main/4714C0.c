/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"



extern void SignalSema(s32);
extern s32 PollSema(s32);
extern char *strncpy(char *, const char *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcGetEntSpace(s32 port, s32 slot, const char *name) {
    sceSifClientData *cd;
    SceMcNameParam *arg;
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
    arg = &_sceMcNameArg;
    arg->port = port;
    arg->slot = slot;
    strncpy(arg->name, name, 0x3FF);
    arg->name[0x3FF] = 0;
    ret = sceSifCallRpc(cd, 0x12, 1, arg, 0x414, &_sceMcRecvBuf, 4, NULL, NULL);
    if (ret == 0) {
        _sceMcCmdNo = 0x12;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
