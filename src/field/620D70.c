#include "common.h"
#include "field/menu.h"

void FldMenuWinAddStyle(s32 arg0, s32 arg1) {
    FldMenuWin *w;

    w = &fldMenuWins[arg0 & 0xFF];
    w->styleFlags = w->styleFlags | arg1;
}
