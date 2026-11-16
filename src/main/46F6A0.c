/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"

extern u8 _sceCdSCmdWaitStr[];


s32 sceCdSyncS(s32 arg0) {
    if (arg0 == 0) {
        if (_sceCdDebug > 0) {
            scePrintf(_sceCdSCmdWaitStr);
        }
        while (sceSifCheckStatRpc(&_sceCdSCmdClient) != 0) {
            _sceCdSleep(0x3C);
        }
        return 0;
    }
    return sceSifCheckStatRpc(&_sceCdSCmdClient);
}
