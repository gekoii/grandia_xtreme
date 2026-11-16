#include "common.h"
#include "field/menu.h"

void FldMenuWinTextClear(u8 arg0) {
    FldMenuWin *w;

    w = &fldMenuWins[arg0];
    w->glyphCount = 0;
}
