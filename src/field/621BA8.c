#include "common.h"
#include "protos/field.h"


void FldMenuWinCloseAll(void) {
    s32 i;
    s32 n = 0;

    for (i = 0; i < 18; i++) {
        if (FldMenuWinGetState(i)) {
            FldMenuWinClose(i, n, 3);
            n += 2;
        }
    }
}
