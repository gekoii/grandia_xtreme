#include "common.h"
#include "field/state.h"
#include "field/menu.h"

s32 FldMenuIsActive(void) {
    s32 r;
    s32 one = 1;

    r = 0;
    if (fldState.mode == one) {
        r = fldMenuWork->openDelay ? 0 : fldState.mode;
    }
    return r;
}
