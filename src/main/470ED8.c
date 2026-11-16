/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"



extern void SignalSema(s32);
extern s32 PollSema(s32);
extern char *strncpy(char *, const char *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcDelete(s32 port, s32 slot, const char *name) {
    sceSifClientData *cd;
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
    strncpy(_sceMcNameArg.name, name, 0x3FF);
    _sceMcNameArg.port = port;
    _sceMcNameArg.slot = slot;
    _sceMcNameArg.name[0x3FF] = 0;
    _sceMcNameArg.flags = 0;
    ret = sceSifCallRpc(cd, 0xF, 1, &_sceMcNameArg, 0x414, &_sceMcRecvBuf, 4, NULL, NULL);
    if (ret == 0) {
        _sceMcCmdNo = 0xF;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
