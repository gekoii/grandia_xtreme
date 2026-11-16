#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtMsgCodeWinNotice(void) {
    if (evtScnrWork->lastWinCode != 2) {
        evtMsgCtl->flags |= 0x2000;
        if (evtScnrCur->id == 0xEF00) {
            evtScnrWork->unk447 = 6;
        } else {
            evtScnrWork->unk447 = 5;
        }
        EvtMsgPromptSet(0, 0, 0);
    }
}
