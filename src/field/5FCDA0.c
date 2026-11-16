#include "common.h"
#include "field/event.h"
#include "protos/field.h"

void EvtMsgLineFeedUpdate(void) {
    if (evtMsgCtl->flags2 & 2) {
        if ((EvtMsgGetCtl()->flags2 & 0x10) || (EvtMsgGetCtl()->flags2 & 8)) {
            EvtMsgGetCtl()->cursorX = 0;
            EvtMsgGetCtl()->cursorY += 2;
        }
        if (EvtMsgGetCtl()->flags2 & 0x10) {
            EvtMsgGetCtl()->cursorY = 0;
        }
        evtMsgCtl->flags2 &= 0xFFE1;
    }
}
