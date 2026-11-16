/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"

s32 sceSifCallRpc(void *, s32, s32, ScePadRpcBuf *, s32, ScePadRpcBuf *, s32, s32, s32);

s32 scePadGetModVersion(void) {
    _scePadRpcBuf.port.command = 0x12;
    if (sceSifCallRpc(&_scePadClient, 1, 0, &_scePadRpcBuf, 0x80, &_scePadRpcBuf, 0x80, 0, 0) < 0) {
        return 0;
    }
    return _scePadRpcBuf.port.result;
}
