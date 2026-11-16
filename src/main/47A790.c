/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"
#include "protos/main.h"


extern s32 sceSifCallRpc(void *, s32, s32, ScePadRpcBuf *, s32, ScePadRpcBuf *, s32, void *, void *);

s32 scePadSetButtonInfo(s32 arg0, s32 arg1, s32 arg2) {
    ScePadRpcBuf *b;

    b = &_scePadRpcBuf;

    b->buttonInfo.command = 10;
    b->buttonInfo.port = arg0;
    b->buttonInfo.slot = arg1;
    b->buttonInfo.mask = arg2;
    if (sceSifCallRpc(_scePadClient, 1, 0, b, 0x80, b, 0x80, NULL, NULL) < 0) {
        return 0;
    }
    if (b->buttonInfo.result == 1) {
        scePadSetReqState(arg0, arg1, 2);
    }
    return b->buttonInfo.result;
}
