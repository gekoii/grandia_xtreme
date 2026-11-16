#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

void FldMenuWinLrArrowsSet(u8 arg0, u8 arg1, f32 arg2) {
    FldMenuWin *e = &fldMenuWins[arg0];

    if (!fldMenuCursor->lrLock) {
        MenuLrArrowsSet(e->openEnd[0], e->openEnd[1] + arg2, (f32)(e->w2 >> 1) + 16.0f);
    }
    fldMenuCursor->lrLock = arg1;
}
