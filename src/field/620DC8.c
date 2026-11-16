#include "common.h"
#include "field/menu.h"

s32 FldMenuWinGetStyle(u8 arg0) {
    FldMenuWin *w;

    w = &fldMenuWins[arg0];
    return w->styleFlags;
}
