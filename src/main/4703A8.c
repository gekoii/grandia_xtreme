/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"



void SignalSema(s32);
s32 PollSema(s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 sceMcSeek(s32 arg0, s32 arg1, s32 arg2) {
    s32 ret;

    if (PollSema(_sceMcSema) < 0) {
        return -0xC8;
    }
    if (_sceMcClient.serve == 0) {
        SignalSema(_sceMcSema);
        return -0x64;
    }
    _sceMcCmdArg.fd = arg0;
    _sceMcCmdArg.offset = arg1;
    _sceMcCmdArg.origin = arg2;
    ret = sceSifCallRpc(&_sceMcClient, 4, 1, &_sceMcCmdArg, 0x30, &_sceMcRecvBuf, 4, 0, 0);
    if (ret == 0) {
        _sceMcCmdNo = 4;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
