/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"




s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 _scePadPortInit(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        _scePadPortTable[0][i].open = 0;
        _scePadPortTable[0][i].unk18 = 0;
        _scePadPortTable[0][i].unk14 = 0;
        _scePadPortTable[1][i].open = 0;
        _scePadPortTable[1][i].unk18 = 0;
        _scePadPortTable[1][i].unk14 = 0;
    }
    _scePadRpcBuf.port.command = 0x10;
    _scePadRpcBuf.port.mode = 0;
    if (sceSifCallRpc(&_scePadClient, 1, 0, &_scePadRpcBuf, 0x80, &_scePadRpcBuf, 0x80, 0, 0) < 0) {
        return 0;
    }
    return _scePadRpcBuf.port.result;
}
