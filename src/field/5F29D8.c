#include "common.h"
#include "party.h"

s32 PtyPartyGetSlot(s16 id) {
    s16 i;
    s32 r = 0;

    for (i = 0; i < 4; i++) {
        if (ptyPartyUnits[i].id - 0x1001 == id) {
            r = i;
            break;
        }
    }
    return r;
}
