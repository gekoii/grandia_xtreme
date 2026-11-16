#include "common.h"
#include "protos/field.h"
#include "field/event.h"


void TxtMsgWinDrawCursors(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (evtMsgWins[i].layout != 0xFF) {
            TxtMsgCursorDraw(i);
        }
    }
}
