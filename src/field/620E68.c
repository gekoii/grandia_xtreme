#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

void FldMenuCursorSetAt(u8 arg0, f32 dx, f32 dy) {
    FldMenuWin *w;
    FldMenuCursor *c;
    f32 x;
    f32 y;

    w = &fldMenuWins[arg0];
    c = fldMenuCursor;
    x = (w->pos[0] + dx) - (f32)(w->w2 >> 1);
    c->x = x;
    y = (w->pos[1] + dy) - (f32)(w->h2 >> 1);
    c->y = y;
    if (c->shown == 0) {
        MenuCursorShow(x, y);
    }
    fldMenuCursor->shown = 1;
}
