#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

void EvtMsgCodePageWait(void) {
    EvtMsgCtl *c;

    c = evtMsgCtl;
    c->flags |= 0xA;
    *evtScnrInput &= 0x9FFF;
    c->flags2 |= 0x12;
    if (!(c->flags & 0x2000)) {
        if (evtScnrWork->winKind == 2) {
            EvtMsgPromptShow(1, 0);
        } else {
            EvtMsgPromptShow(1, 1);
        }
    }
    evtMsgCtl->unk24 = 0;
}
