/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "protos/main.h"



extern void SignalSema(s32);
extern s32 PollSema(s32);
extern void sceSifWriteBackDCache(void *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceMcGetInfo(s32 port, s32 slot, s32 *type, s32 *free, s32 *format) {
    s32 ret;

    if (PollSema(_sceMcSema) < 0) {
        return -0xC8;
    }
    if (_sceMcClient.serve == 0) {
        SignalSema(_sceMcSema);
        return -0x64;
    }
    _sceMcCmdArg.port = port;
    _sceMcCmdArg.slot = slot;
    _sceMcCmdArg.param = _sceMcRecvInfoBuf;
    if (type != NULL) {
        _sceMcCmdArg.origin = 1;
    } else {
        _sceMcCmdArg.origin = 0;
    }
    if (free != NULL) {
        _sceMcCmdArg.offset = 1;
    } else {
        _sceMcCmdArg.offset = 0;
    }
    if (format != NULL) {
        _sceMcCmdArg.size = 1;
    } else {
        _sceMcCmdArg.size = 0;
    }
    _sceMcGetInfoType = type;
    _sceMcGetInfoFree = free;
    _sceMcGetInfoFormat = format;
    sceSifWriteBackDCache(_sceMcRecvInfoBuf, 0xC0);
    ret = sceSifCallRpc(&_sceMcClient, 1, 1, &_sceMcCmdArg, 0x30, &_sceMcRecvBuf, 4, _sceMcGetInfoApdx, _sceMcRecvInfoBuf);
    if (ret == 0) {
        _sceMcCmdNo = 1;
    } else {
        SignalSema(_sceMcSema);
    }
    return ret;
}
