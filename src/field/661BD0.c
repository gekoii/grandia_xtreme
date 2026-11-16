#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtMsgChoiceRedraw(void) {
    u8 saved;

    saved = evtMsgCtl->flags;
    evtScnrWork->pc = evtMsgCtl->unk34;
    evtMsgCtl->flags = (evtMsgCtl->flags & ~4) | 0x4000;
    evtMsgCtl->cursorY = 0;
    evtMsgCtl->cursorX = 0;
    EvtMsgAnalyze();
    evtMsgCtl->flags = saved;
}
