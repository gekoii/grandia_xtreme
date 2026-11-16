/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"
#include "protos/main.h"


extern s32 sceSifCallRpc(void *, s32, s32, ScePadRpcBuf *, s32, ScePadRpcBuf *, s32, void *, void *);

s32 scePadSetActAlign(s32 arg0, s32 arg1, u8 *arg2) {
    s32 i;
    ScePadRpcBuf *b;

    b = &_scePadRpcBuf;

    b->actAlign.command = 8;
    b->actAlign.port = arg0;
    b->actAlign.slot = arg1;
    for (i = 0; i < 6; i++) {
        b->actAlign.align[i] = arg2[i];
    }
    if (sceSifCallRpc(_scePadClient, 1, 0, b, 0x80, b, 0x80, NULL, NULL) < 0) {
        return 0;
    }
    if (b->actAlign.result == 1) {
        scePadSetReqState(arg0, arg1, 2);
    }
    return b->actAlign.result;
}
