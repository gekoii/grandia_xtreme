#include "common.h"
#include "field/menu.h"

void FldMenuWinSetActive(u8 arg0) {
    s32 i;
    FldMenuWin *e = fldMenuWins;

    for (i = 0; i < 18; i++, e++) {
        if (e->flags & 0x8000) {
            if (i == arg0) {
                e->selected = 1;
            } else {
                e->selected = 0;
            }
        }
    }
}
