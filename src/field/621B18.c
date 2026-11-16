#include "common.h"
#include "field/menu.h"

void FldMenuWinClearPortrait(u8 arg0) {
    FldMenuWin *w = &fldMenuWins[arg0];

    w->unk16 = 0;
}
