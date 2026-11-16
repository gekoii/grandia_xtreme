#include "common.h"
#include "field/menu.h"

void FldMenuWinMoveTo(s32 arg0, s16 arg1, s16 arg2) {
    FldMenuWin *w;

    w = &fldMenuWins[arg0 & 0xFF];
    if (w->active != 0 && w->state == 4) {
        w->pos[3] = 0;
        w->pos[2] = 0;
        w->pos[0] = arg1;
        w->pos[1] = arg2;
    }
}
