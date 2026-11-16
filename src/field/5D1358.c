#include "common.h"
#include "field/event.h"
#include "field/n15.h"

s32 TxtMsgCursorIsVisible(void) {
    if ((evtScnrCur->id == 0xFFFF) || !(evtMsgCtl->flags & 8)) {
        return 0;
    }
    return evtMsgCursor->on != 0;
}
