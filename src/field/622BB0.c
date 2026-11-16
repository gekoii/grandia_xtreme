#include "common.h"
#include "field/menu.h"

void FldMenuWinGetOrigin(u8 arg0, s32 *arg1, s32 *arg2) {
    FldMenuWin *e = &fldMenuWins[arg0];

    *arg1 = (s32)(e->pos[0] + 0.5f) - (e->w2 >> 1);
    *arg2 = (s32)(e->pos[1] + 0.5f) - (e->h2 >> 1);
}
