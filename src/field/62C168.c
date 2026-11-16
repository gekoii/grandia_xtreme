#include "common.h"
#include "save.h"
#include "field/n23.h"

s32 FldMenuPartyOrderIsComplete(s32 arg0) {
    s32 i;

    if (evtMainFlag >= 0x3CD2 || fldMenuPartyOrder.count < 4) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        if (fldMenuPartyOrder.slots[i] == 0) {
            return 0;
        }
    }
    return 1;
}
