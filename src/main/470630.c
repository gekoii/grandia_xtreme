/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"



extern void SignalSema(s32);
extern s32 PollSema(s32);
extern void FlushCache(s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcWrite(s32 fd, u8 *buf, s32 size) {
    s32 ret;
    s32 n;
    u32 i;
    u32 a;
    u32 b;

    if (PollSema(_sceMcSema) < 0) {
        return -0xC8;
    }
    if (_sceMcClient.serve == 0) {
        SignalSema(_sceMcSema);
        return -0x64;
    }
    _sceMcCmdArg.fd = fd;
    if (size <= 16) {
        _sceMcCmdArg.origin = size;
        _sceMcCmdArg.size = 0;
        _sceMcCmdArg.buffer = 0;
    } else {
        a = (u32)(buf - 1) & ~0xF;
        b = (u32)buf - 16;
        n = a - b;
        _sceMcCmdArg.size = size - n;
        _sceMcCmdArg.origin = n;
        _sceMcCmdArg.buffer = buf + n;
    }
    for (i = 0; i < _sceMcCmdArg.origin; i++) {
        _sceMcCmdArg.data[i] = buf[i];
    }
    FlushCache(0);
    ret = sceSifCallRpc(&_sceMcClient, 6, 1, &_sceMcCmdArg, 0x30, &_sceMcRecvBuf, 4, NULL, NULL);
    if (ret == 0) {
        _sceMcCmdNo = 6;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
