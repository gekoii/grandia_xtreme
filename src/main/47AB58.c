/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"


s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 _scePadRpcCmd14(s32 arg0) {
    _scePadRpcBuf.cmd14.arg = arg0;
    _scePadRpcBuf.cmd14.command = 0x14;
    if (sceSifCallRpc(_scePadClient, 1, 0, &_scePadRpcBuf, 0x80, &_scePadRpcBuf, 0x80, 0, 0) < 0) {
        return 0;
    }
    return _scePadRpcBuf.cmd14.result;
}
