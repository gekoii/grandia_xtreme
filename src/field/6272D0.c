#include "common.h"
#include "protos/field.h"


void FldMenuItemExit(void) {
    s32 i;

    for (i = 8; i < 0xF; i++) {
        FldMenuWinClose(i, 0, 3);
    }
    FldMenuWinLrArrowsSet(8, 0, 0.0f);
}
