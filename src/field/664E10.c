#include "common.h"
#include "field/event.h"

typedef struct {
    u16 idx;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} Rec;

extern Rec evtMsgLayoutTable[];

void EvtMsgWinPlace(u8 type, s32 *out, u16 h, u16 w) {
    EvtMsgWin *win;
    s32 n;

    win = evtMsgWins;
    win += evtMsgLayoutTable[type].idx;
    win->flags = 1;
    win->layout = type;
    win->x0 = evtMsgLayoutTable[type].x0;
    win->y0 = evtMsgLayoutTable[type].y0;
    win->x1 = evtMsgLayoutTable[type].x1;
    win->y1 = evtMsgLayoutTable[type].y1;
    switch (type) {
    case 2:
        n = w >> 4;
        if (w & 0xF) n++;
        win->x1 = win->x0 + n + 3;
        win->y1 = win->y0 + (((h * 24) >> 4) + (h & 1) + 1);
        break;
    case 3:
        win->x0 = win->x1 - (w >> 4);
        if (w & 0xF) {
            win->x0 -= 1;
        }
        win->x0 -= 2;
        win->y1 = win->y0 + (((h * 24) >> 4) + (h & 1) + 1);
        break;
    case 4:
    case 5:
        n = w >> 4;
        if (w & 0xF) n++;
        win->x0 = evtMsgLayoutTable[type].x0 - (n >> 1) - (n & 1);
        win->x1 = evtMsgLayoutTable[type].x1 + (n >> 1) + 1;
        win->y1 = evtMsgLayoutTable[type].y0 + 1 + (((h * 24) >> 4) + (h & 1));
        break;
    case 6:
        n = w >> 4;
        if (w & 0xF) n++;
        win->x0 = evtMsgLayoutTable[type].x0 - n - 1;
        win->x1 = evtMsgLayoutTable[type].x1;
        win->y1 = evtMsgLayoutTable[type].y0 + 1 + (((h * 24) >> 4) + (h & 1));
        break;
    }
    switch (type) {
    case 0:
    case 1:
        win->width = 0x230;
        win->lines = 3;
        break;
    case 2:
        win->lines = h;
        win->width = ((win->x1 - win->x0) - 2) * 16;
        break;
    default:
        win->width = w;
        win->lines = h;
        break;
    }
    out[0] = win->x0;
    out[1] = win->y0;
    out[2] = win->x1;
    out[3] = win->y1;
}
