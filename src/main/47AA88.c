/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"


s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 scePadGetSlotMax(s32 arg0) {
    _scePadRpcBuf.port.port = arg0;
    _scePadRpcBuf.port.command = 13;
    if (sceSifCallRpc(_scePadClient, 1, 0, &_scePadRpcBuf, 0x80, &_scePadRpcBuf, 0x80, 0, 0) < 0) {
        return 0;
    }
    return _scePadRpcBuf.port.result;
}
