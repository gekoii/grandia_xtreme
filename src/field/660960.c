#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtScnrEnd(void) {
    evtScnrWork->pc = 0;
    evtScnrWork->winKind = 0;
    evtScnrCur->flags = 2;
    evtMsgCtl->state = 0xFF;
    evtScnrCur->id = 0xFFFF;
    evtScnrCur->flags |= 0x20;
    if ((evtMsgWins[0].flags & 2) || (evtMsgWins[1].flags & 2)) {
        EvtMsgWinCloseAll();
        EvtPictureReqsReset();
        EvtMsgWinUpdate();
    }
}
