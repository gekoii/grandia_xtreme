/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "protos/main.h"



extern void SignalSema(s32);
extern s32 PollSema(s32);
extern void sceSifWriteBackDCache(void *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcRead(s32 port, void *buf, s32 size) {
    sceSifClientData *cd;
    SceMcDescParam *arg;
    u8 *info;
    s32 ret;

    if (PollSema(_sceMcSema) < 0) {
        return -0xC8;
    }
    cd = &_sceMcClient;
    if (cd->serve == 0) {
        SignalSema(_sceMcSema);
        return -0x64;
    }
    info = _sceMcRecvInfoBuf;
    arg = &_sceMcCmdArg;
    arg->fd = port;
    arg->param = info;
    arg->buffer = buf;
    arg->size = size;
    sceSifWriteBackDCache(buf, size);
    sceSifWriteBackDCache(info, 0xC0);
    ret = sceSifCallRpc(cd, 5, 1, arg, 0x30, &_sceMcRecvBuf, 4, _sceMcReadFixAlign, info);
    if (ret == 0) {
        _sceMcCmdNo = 5;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
