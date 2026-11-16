/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"

extern void SignalSema(s32);
extern void sceSifWriteBackDCache(s32 *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceCdMmode(s32 arg0) {
    s32 r;
    s32 *p;

    p = _sceCdSCmdSendBuf;
    if (_sceCdCheckSCmd(0x22) == 0) {
        return 0;
    }
    *p = arg0;
    sceSifWriteBackDCache(p, 4);
    if (sceSifCallRpc(&_sceCdSCmdClient, 0x22, 0, p, 4, &_sceCdSCmdRecvBuf, 4, NULL, NULL) < 0) {
        SignalSema(_sceCdSCmdSema);
        return 0;
    }
    r = *(s32 *)((u32)&_sceCdSCmdRecvBuf | 0x20000000);
    SignalSema(_sceCdSCmdSema);
    return r;
}
