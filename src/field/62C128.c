#include "common.h"
#include "field/n23.h"

s32 FldMenuPartyOrderHasChar(u8 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (fldMenuPartyOrderSlots[i] == arg0) {
            return 1;
        }
    }
    return 0;
}
