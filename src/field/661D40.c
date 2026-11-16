#include "common.h"
#include "field/event.h"

void EvtMsgCodeNewLine(void) {
    u16 f;

    f = evtMsgCtl->flags;
    evtMsgCtl->cursorX = 0;
    evtMsgCtl->cursorY += 2;
    if (!(f & 0x1060) && evtMsgCtl->unk24 == 0) {
        evtMsgCtl->flags = f | 0x12;
        evtMsgCtl->wait = evtMsgCtl->unk22;
    }
}
