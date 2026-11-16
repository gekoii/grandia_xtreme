#include "common.h"
#include "protos/field.h"


void FldMenuTopExit(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        FldMenuWinClose(i, i * 2, 3);
    }
}
