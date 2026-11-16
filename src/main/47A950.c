/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"
#include "protos/main.h"



extern s32 sceSifCallRpc(void *, s32, s32, ScePadRpcBuf *, s32, ScePadRpcBuf *, s32, void *, void *);

s32 scePadSetVrefParam(s32 arg0, s32 arg1, ScePadVrefParam *arg2) {
    ScePadRpcBuf *b;

    b = &_scePadRpcBuf;

    b->vref.command = 11;
    b->vref.port = arg0;
    b->vref.slot = arg1;
    b->vref.param = *arg2;
    if (sceSifCallRpc(_scePadClient, 1, 0, b, 0x80, b, 0x80, NULL, NULL) < 0) {
        return 0;
    }
    if (b->vref.result == 1) {
        scePadSetReqState(arg0, arg1, 2);
    }
    return b->vref.result;
}
