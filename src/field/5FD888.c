#include "common.h"
#include "protos/field.h"

void EvtScnrCall(s32 arg0) {
    /* EvtScnrStart's result compared signed (slti): through an s32 cast, its u16 return gives sltiu */
    if ((((s32 (*)(u16))EvtScnrStart)(arg0 & 0xFFFF) < 2) && (EvtScnrHighIdLatch() == 0)) {
        TxtGlyphLayoutClear(3);
        TxtGlyphLayoutClear(4);
        TxtMsgWinClose(0);
        TxtMsgWinClose(1);
        EvtFaceWinCloseAll();
    }
}
