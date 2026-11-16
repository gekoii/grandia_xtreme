/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"


s32 ReferThreadStatus(s32, void *);
s32 SignalSema(s32);
s32 PollSema(s32);
s32 sceSifBindRpc(sceSifClientData *, u32, s32);
extern char _sceCdNCmdSemaFailFmt[];
extern char _sceCdNCmdBindErrStr[];

s32 _sceCdCheckNCmd(s32 arg0) {
    s32 i;

    _sceCdSemaInit();
    if (_sceCdNCmdSema != PollSema(_sceCdNCmdSema)) {
        if (_sceCdDebug > 0) {
            scePrintf(_sceCdNCmdSemaFailFmt, arg0, _sceCdNCmdNum);
        }
        return 0;
    }
    _sceCdNCmdNum = arg0;
    ReferThreadStatus(_sceCdThreadId, &_sceCdThreadStatus);
    if (sceCdSync(1) != 0) {
        SignalSema(_sceCdNCmdSema);
        return 0;
    }
    sceSifInitRpc(0);
    i = _sceCdNCmdBind;
    if (i < 0) {
        while (1) {
            if (sceSifBindRpc(&_sceCdNCmdClient, 0x80000595, 0) < 0) {
                if (_sceCdDebug > 0) {
                    scePrintf(_sceCdNCmdBindErrStr);
                }
                for (i = 0x100000; i != -1; i--) {
                }
                continue;
            }
            if (_sceCdNCmdClient.serve != 0) {
                break;
            }
            for (i = 0x100000; i != -1; i--) {
            }
        }
        _sceCdNCmdBind = 0;
    }
    return 1;
}
