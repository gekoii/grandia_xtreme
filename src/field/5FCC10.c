#include "common.h"
#include "field/event.h"
#include "protos/field.h"

void EvtMsgClearRequestB(void) {
    if (EvtScnrGetCur()->flags & 0x80) {
        EvtMsgLayoutClear(0);
        evtScnrCur->flags &= 0xFF7F;
    }
    if (EvtScnrGetCur()->flags & 0x100) {
        EvtMsgLayoutClear(1);
        evtScnrCur->flags &= 0xFEFF;
    }
}
