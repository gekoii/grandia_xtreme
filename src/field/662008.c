#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtMsgCodeWinSelect(void) {
    EvtMsgCtl *c;
    u16 f;

    c = evtMsgCtl;
    c->cursorX = 0;
    c->cursorY = 0;
    f = c->flags | 0x10A;
    c->flags = f;
    if (!(f & 0x800)) {
        if (evtScnrWork->winKind == 2) {
            c->unkE = 0x13;
            ((s32 (*)(u16, u16))EvtMsgPromptShow)(1, 0);
        } else {
            c->unkE = 1;
            ((s32 (*)(u16, u16))EvtMsgPromptShow)(1, 1);
        }
    }
}
