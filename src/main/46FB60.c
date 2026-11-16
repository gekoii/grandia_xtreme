/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"


s32 SignalSema(s32);
s32 PollSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifBindRpc(sceSifClientData *, u32, s32);
s32 sceSifCallRpc(sceSifClientData *, s32, s32, void *, s32, void *, s32, void *, void *);
extern char _sceCdDiskReadyStartStr[];
extern char _sceCdDiskReadyBindErrStr[];
extern char _sceCdDiskReadyEndStr[];

#define UNC(x) ((u32)(x) | 0x20000000)
s32 sceCdDiskReady(s32 arg0) {
    s32 ret;
    s32 i;
    s32 *buf;

    if (_sceCdDebug > 0) {
        scePrintf(_sceCdDiskReadyStartStr);
    }
    _sceCdSemaInit();
    if (_sceCdSCmdSema != PollSema(_sceCdSCmdSema)) {
        return 6;
    }
    if (sceCdSyncS(1) != 0) {
        SignalSema(_sceCdSCmdSema);
        return (arg0 != 8) ? 6 : -1;
    }
    do {
        sceSifInitRpc(0);
        i = _sceCdDiskReadyBind;
    } while (0);
    if (i < 0) {
        while (1) {
            if (sceSifBindRpc(&_sceCdDiskReadyClient, 0x8000059A, 0) < 0) {
                if (_sceCdDebug > 0) {
                    scePrintf(_sceCdDiskReadyBindErrStr);
                }
                for (i = 0x100000; i != -1; i--) {
                }
                continue;
            }
            if (_sceCdDiskReadyClient.serve != 0) {
                break;
            }
            for (i = 0x100000; i != -1; i--) {
            }
        }
        _sceCdDiskReadyBind = 0;
    }
    _sceCdDiskReadySendBuf = arg0;
    sceSifWriteBackDCache(&_sceCdDiskReadySendBuf, 4);
    buf = _sceCdSCmdRecvBuf;
    if (sceSifCallRpc(&_sceCdDiskReadyClient, 0, 0, &_sceCdDiskReadySendBuf, 4, buf, 4, NULL, NULL) < 0) {
        SignalSema(_sceCdSCmdSema);
        return (arg0 != 8) ? 6 : -1;
    }
    if (_sceCdDebug > 0) {
        scePrintf(_sceCdDiskReadyEndStr);
    }
    ret = *(s32 *)UNC(buf);
    SignalSema(_sceCdSCmdSema);
    return ret;
}
