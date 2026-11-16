/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"



void SignalSema(s32);
s32 PollSema(s32);
s32 sceSifCallRpc(sceSifClientData *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcFormat(s32 arg0, s32 arg1) {
    s32 ret;

    if (PollSema(_sceMcSema) < 0) {
        return -200;
    }
    if (_sceMcClient.serve == 0) {
        SignalSema(_sceMcSema);
        return -100;
    }
    _sceMcCmdArg.slot = arg1;
    _sceMcCmdArg.port = arg0;
    ret = sceSifCallRpc(&_sceMcClient, 0x10, 1, &_sceMcCmdArg, 0x30, &_sceMcRecvBuf, 4, NULL, NULL);
    if (ret == 0) {
        _sceMcCmdNo = 0x10;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
