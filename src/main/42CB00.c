#include "common.h"
#include "save.h"
#include "main/n03.h"

void McSlotCopyHeader(SysSaveData *a, McSlot *b) {
    s32 i;

    b->playTime = a->playTime;
    b->unk2 = a->unk20;
    for (i = 0; i < 4; i++) {
        b->party[i] = a->headParty[i];
    }
    for (i = 0; i < 4; i++) {
        b->level[i] = a->headLevel[i];
    }
    b->partyCount = 0;
    /* do-while(0): scheduling barrier, the partyCount = 0 store stays before the tests */
    do {
        if (b->party[1] == 0) {
            b->partyCount = 1;
        } else if (b->party[2] == 0) {
            b->partyCount = 2;
        } else if (b->party[3] == 0) {
            b->partyCount = 3;
        } else {
            b->partyCount = 4;
        }
    } while (0);
}
