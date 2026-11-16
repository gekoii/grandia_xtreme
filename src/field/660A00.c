#include "common.h"
#include "field/event.h"

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
} Src;

s32 EvtMsgCtlStart(Src *arg0, s32 arg1) {
    if (evtMsgCtl->state == 0xFF) {
        evtMsgCtl->state = 0;
        if (evtScnrWork->winKind == 1 || evtScnrWork->winKind == 2) {
            evtMsgCtl->flags = 1;
        } else if (evtScnrWork->winKind == 0x80) {
            /* identical arms: the original still tests the condition (the branch is in the code) */
            if (evtScnrCur->noticeTimer) {
                evtMsgCtl->flags = 0x1001;
            } else {
                evtMsgCtl->flags = 0x1001;
            }
        }
        evtMsgCtl->unk10 = arg0->unk8;
        evtMsgCtl->unk12 = arg0->unkC;
        evtMsgCtl->cursorX = 0;
        evtMsgCtl->cursorY = 0;
        evtMsgCtl->choice = 0;
        evtMsgCtl->wait = 0;
        evtMsgCtl->textPc = (u8 *)arg1;
        return 0;
    }
    return -1;
}
