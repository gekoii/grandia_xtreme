#include "common.h"
#include "field/menu.h"

void FldMenuWinSetFlags(u8 arg0, s32 arg1) {
    FldMenuWin *w = &fldMenuWins[arg0];

    w->flags = arg1;
}
