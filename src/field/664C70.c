#include "common.h"
#include "protos/field.h"
#include "field/event.h"


void EvtMsgWinTilesUpdate(void) {
    s32 i;
    s32 x0, y0, x1, y1;

    for (i = 0; i < 2; i++) {
        if (evtMsgWins[i].layout != 0xFF) {
            if (evtMsgWins[i].flags & 0x80) {
                if (evtMsgWins[i].flags & 8) {
                    evtMsgWins[i].layout = 0xFF;
                    evtMsgWins[i].flags = 0;
                    EvtMsgTileRectClear(evtMsgWins[i].x0, evtMsgWins[i].y0, evtMsgWins[i].x1, evtMsgWins[i].y1);
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (evtMsgWins[i].layout != 0xFF) {
            if (evtMsgWins[i].flags & 0x80) {
                if (!(evtMsgWins[i].flags & 8)) {
                    evtMsgWins[i].flags &= 0x7F;
                    x0 = evtMsgWins[i].x0;
                    y0 = evtMsgWins[i].y0;
                    x1 = evtMsgWins[i].x1;
                    y1 = evtMsgWins[i].y1;
                    if (evtMsgWins[i].layout == 3) {
                        EvtMsgTileFramePlain(x0, y0, x1, y1);
                    } else {
                        EvtMsgTileFrame(x0, y0, x1, y1);
                    }
                }
            }
        }
    }
}
