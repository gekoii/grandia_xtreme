#include "common.h"
#include "field/event.h"

void EvtMsgWinCloseAll(void) {
    EvtMsgWin *p = evtMsgWins;
    s32 ff = 0xFF;
    s32 ten = 10;
    s32 i;

    for (i = 1; i >= 0; i--) {
        if (p->layout != ff) {
            p->flags = ten;
        }
        p++;
    }
}
