/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"


extern void SignalSema(s32);
extern s32 PollSema(s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcFlush(s32 arg0) {
    s32 ret;

    if (PollSema(_sceMcSema) < 0) {
        return -0xC8;
    }
    if (_sceMcClient.serve == 0) {
        SignalSema(_sceMcSema);
        return -0x64;
    }
    _sceMcCmdArg.fd = arg0;
    ret = sceSifCallRpc(&_sceMcClient, 0xA, 1, &_sceMcCmdArg, 0x30, &_sceMcRecvBuf, 4, NULL, NULL);
    if (ret == 0) {
        _sceMcCmdNo = 0xA;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
