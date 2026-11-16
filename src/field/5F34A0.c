#include "common.h"
#include "party.h"

void PtyClearSp(void) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        ptyCharData[i].sp = 0;
    }
    for (i = 0; i < 4; i++) {
        if (ptyPartyUnits[i].id != 0) {
            ptyPartyUnits[i].sp = 0;
            ptyPartyUnits[i].angry = 0;
        }
    }
}
