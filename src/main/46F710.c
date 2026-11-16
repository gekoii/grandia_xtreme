/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"


s32 ReferThreadStatus(s32, void *);
s32 SignalSema(s32);
s32 PollSema(s32);
s32 sceSifBindRpc(sceSifClientData *, u32, s32);
extern char _sceCdSCmdSemaFailFmt[];
extern char _sceCdSCmdBindErrStr[];

s32 _sceCdCheckSCmd(s32 arg0) {
    s32 i;

    _sceCdSemaInit();
    if (_sceCdSCmdSema != PollSema(_sceCdSCmdSema)) {
        if (_sceCdDebug > 0) {
            scePrintf(_sceCdSCmdSemaFailFmt, arg0, _sceCdSCmdNum);
        }
        return 0;
    }
    _sceCdSCmdNum = arg0;
    ReferThreadStatus(_sceCdThreadId, &_sceCdThreadStatus);
    if (sceCdSyncS(1) != 0) {
        SignalSema(_sceCdSCmdSema);
        return 0;
    }
    sceSifInitRpc(0);
    i = _sceCdSCmdBind;
    if (i < 0) {
        while (1) {
            if (sceSifBindRpc(&_sceCdSCmdClient, 0x80000593, 0) < 0) {
                if (_sceCdDebug > 0) {
                    scePrintf(_sceCdSCmdBindErrStr);
                }
                for (i = 0x100000; i != -1; i--) {
                }
                continue;
            }
            if (_sceCdSCmdClient.serve != 0) {
                break;
            }
            for (i = 0x100000; i != -1; i--) {
            }
        }
        _sceCdSCmdBind = 0;
    }
    return 1;
}
