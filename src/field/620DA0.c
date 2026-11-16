#include "common.h"
#include "field/menu.h"

void FldMenuWinSetScrollArrows(u8 arg0, u8 arg1, u8 arg2) {
    FldMenuWin *w;

    w = &fldMenuWins[arg0];
    w->arrowL = arg1;
    w->arrowR = arg2;
}
