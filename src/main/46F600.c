/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"

extern u8 _sceCdNCmdWaitStr[];


s32 sceCdSync(s32 arg0) {
    if (arg0 == 0) {
        if (_sceCdDebug > 0) {
            scePrintf(_sceCdNCmdWaitStr);
        }
        while (_sceCdNCmdBusy != 0 || sceSifCheckStatRpc(&_sceCdNCmdClient) != 0) {
            _sceCdSleep(0x3C);
        }
        return 0;
    }
    if (_sceCdNCmdBusy != 0 || sceSifCheckStatRpc(&_sceCdNCmdClient) != 0) {
        return 1;
    }
    return 0;
}
