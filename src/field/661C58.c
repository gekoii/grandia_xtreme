#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

void EvtMsgCodeWaitButton(void) {
    EvtMsgCtl *c;
    EvtScnrWork *s;
    u16 *f;

    c = evtMsgCtl;
    f = evtScnrInput;
    s = evtScnrWork;
    c->flags |= 0x800A;
    *f &= 0x9FFF;
    if (s->winKind == 2) {
        EvtMsgPromptShow(1, 0);
    } else {
        EvtMsgPromptShow(1, 1);
    }
}
