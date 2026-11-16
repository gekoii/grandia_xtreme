/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"
#include "protos/main.h"


s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 scePadSetMainMode(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    _scePadRpcBuf.mainMode.offs = arg2;
    _scePadRpcBuf.mainMode.lock = arg3;
    _scePadRpcBuf.mainMode.command = 6;
    _scePadRpcBuf.mainMode.port = arg0;
    _scePadRpcBuf.mainMode.slot = arg1;
    if (sceSifCallRpc(_scePadClient, 1, 0, &_scePadRpcBuf, 0x80, &_scePadRpcBuf, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (_scePadRpcBuf.mainMode.result == 1) {
        scePadSetReqState(arg0, arg1, 2);
    }
    return _scePadRpcBuf.mainMode.result;
}
