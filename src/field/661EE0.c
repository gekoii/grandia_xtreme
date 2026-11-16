#include "common.h"
#include "protos/field.h"
#include "field/event.h"


u8 *EvtMsgCodeChoice(u8 *p) {
    s32 out[4];
    u8 lo;
    u8 hi;

    if (evtScnrCur->winKind == 2) {
        return p;
    }
    {
        evtMsgCtl->cursorX = 0;
        evtMsgCtl->cursorY = 0;
        evtMsgCtl->choice = 0;
        lo = *p++;
        hi = *p++;
        evtMsgCtl->choiceCount = hi >> 5;
        EvtMsgWinPlace(2, out, hi >> 5, lo | ((hi & 0x1F) << 8));
        evtMsgCtl->unkC = out[0];
        evtMsgCtl->unkE = out[1];
        evtMsgCtl->flags |= 0x21;
        evtMsgCtl->unk34 = evtScnrWork->pc + 2;
        EvtMsgPromptSet(0, 0, 0);
    }
    return p;
}
