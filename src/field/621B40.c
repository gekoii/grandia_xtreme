#include "common.h"
#include "field/menu.h"

s32 FldMenuWinAllClosed(void) {
    s32 i;
    FldMenuWin *e = fldMenuWins;

    for (i = 0; i < 18; i++, e++) {
        if (e->active == 1) {
            return 0;
        }
    }
    return 1;
}
