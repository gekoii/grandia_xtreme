#include "common.h"
#include "field/menu.h"

s32 FldMenuWinGetState(u8 arg0) {
    FldMenuWin *w = &fldMenuWins[arg0];

    return w->state;
}
