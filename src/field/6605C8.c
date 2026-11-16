#include "common.h"
#include "field/event.h"

void EvtMsgCursorReset(void) {
    evtMsgCursor->on = 0;
    evtMsgCursor->unk1 = 0;
    evtMsgCursor->x = 0;
    evtMsgCursor->y = 0;
}
