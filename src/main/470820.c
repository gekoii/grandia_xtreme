/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "protos/main.h"

void SignalSema(s32);

s32 sceMcSync(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 r;

    if (_sceMcCmdNo == 0) {
        return -1;
    }
    r = sceSifCheckStatRpc(&_sceMcClient);
    if (arg0 == 0 && r != 0) {
        while (sceSifCheckStatRpc(&_sceMcClient) != 0) {
            _sceMcSleep(60);
        }
        r = 0;
    }
    r = (r == 0);
    if (arg1 != NULL) {
        *arg1 = _sceMcCmdNo;
    }
    if (r) {
        _sceMcCmdNo = 0;
        if (arg2 != NULL) {
            *arg2 = _sceMcRecvBuf.result;
        }
        SignalSema(_sceMcSema);
    }
    return r;
}
