/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"





s32 scePadSetActDirect(s32 arg0, s32 arg1, u8 *arg2) {
    ScePadActBuf *pkt;
    s32 i;

    if (_scePadGetDmaStr(arg0, arg1)->currentTask != 1) {
        return 0;
    }
    pkt = _scePadPortTable[arg0][arg1].actBuf;
    for (i = 0; i < 6; i++) {
        pkt->data[i] = arg2[i];
    }
    pkt->unk8 = 6;
    pkt->unk4 = 1;
    _scePadSendActDirect(arg0, arg1);
    return 1;
}
