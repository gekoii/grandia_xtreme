#include "common.h"
#include "field/event.h"
#include "protos/field.h"

#define CUR EvtScnrGetCur()

void EvtMsgClearRequestA(void) {
    EvtScnrCur *c;

    if (CUR->flags & 0x60) {
        if ((CUR->flags & 0x20) && (CUR->flags & 0x40)) {
            EvtMsgLayoutClear(0);
            EvtMsgLayoutClear(1);
        } else if (CUR->flags & 0x20) {
            EvtMsgLayoutClear(0);
        } else {
            EvtMsgLayoutClear(1);
        }
        c = CUR;
        c->flags &= 0xFF9F;
    }
}
