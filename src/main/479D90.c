/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"



s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 scePadPortClose(s32 arg0, s32 arg1) {
    ScePadRpcBuf *r;

    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0;
    }
    r = &_scePadRpcBuf;
    r->port.command = 0xE;
    r->port.port = arg0;
    r->port.slot = arg1;
    r->port.mode = 1;
    if (sceSifCallRpc(&_scePadClient, 1, 0, r, 0x80, r, 0x80, 0, 0) < 0) {
        return 0;
    }
    _scePadPortTable[arg0][arg1].open = 0;
    return r->port.result;
}
