/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"



extern void SignalSema(s32);
extern s32 PollSema(s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcChangeThreadPriority(s32 arg0) {
    s32 r;

    if (PollSema(_sceMcSema) < 0) {
        return -200;
    }
    if (_sceMcClient.serve == 0) {
        SignalSema(_sceMcSema);
        return -100;
    }
    _sceMcCmdArg.origin = arg0;
    r = sceSifCallRpc(&_sceMcClient, 0x14, 1, &_sceMcCmdArg, 0x30, &_sceMcRecvBuf, 4, NULL, NULL);
    if (r == 0) {
        _sceMcCmdNo = 0x14;
    } else {
        SignalSema(_sceMcSema);
    }
    return r;
}
