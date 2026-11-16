#include "common.h"
#include "party.h"






void PtyRestoreAll(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        ptyCharData[i].sp = 0;
        ptyCharData[i].hp = ptyCharData[i].maxHp;
        ptyCharData[i].status = 0;
    }
    for (i = 0; i < 4; i++) {
        if (ptyPartyUnits[i].id != 0) {
            ptyPartyUnits[i].hp = ptyPartyUnits[i].maxHp;
            ptyPartyUnits[i].mp = ptyPartyUnits[i].maxMp;
            ptyPartyUnits[i].status = 0;
        }
    }
    for (i = 0; i < 24; i++) {
        if (ptyPartyData->eggs[i].itemId != 0) {
            ptyPartyData->eggs[i].mp = ptyPartyData->eggs[i].maxMp;
        }
    }
}
