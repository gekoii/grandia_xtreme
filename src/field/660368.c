#include "common.h"
#include "field/event.h"

void EvtMsgWindowsReset(void) {
    s32 i;
    EvtMsgWin *p;
    s32 ff;

    ff = 0xFF;
    p = evtMsgWins;
    for (i = 1; i >= 0; i--) {
        p->layout = ff;
        p->flags = 0;
        p->x0 = 0;
        p->y0 = 0;
        p->x1 = 0;
        p->y1 = 0;
        p->width = 0;
        p->lines = 0;
        p++;
    }
}
