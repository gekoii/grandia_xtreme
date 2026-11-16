#include "common.h"
#include "field/event.h"

void EvtMsgPromptSet(s8 arg0, s16 arg1, s16 arg2) {
    evtMsgCursor->on = arg0;
    evtMsgCursor->x = arg1;
    evtMsgCursor->y = arg2;
    evtMsgCursor->unk1 = 0;
}
