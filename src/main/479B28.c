/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"


extern s32 sceSifCallRpc(void *, s32, s32, ScePadRpcBuf *, s32, ScePadRpcBuf *, s32, void *, void *);

s32 scePadEnd(void) {
    _scePadRpcBuf.port.command = 0xF;
    if (sceSifCallRpc(_scePadClient, 1, 0, &_scePadRpcBuf, 0x80, &_scePadRpcBuf, 0x80, NULL, NULL) < 0) {
        return 0;
    }
    if (_scePadRpcBuf.port.result == 1) {
        _scePadInitDone = 0;
    }
    return _scePadRpcBuf.port.result;
}
