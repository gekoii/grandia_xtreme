#include "common.h"
#include "party.h"



void MemSet(void *, s32, s32);

void ItemInventoryClear(void) {
    s32 i;
    s32 j;
    s32 m;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 6; j++) {
            ptyCharData[i].equip[j] = 0;
        }
        m = -1;
        for (j = 0; j < 6; j++) {
            ptyCharData[i].slots[j] = m;
        }
        m = -1;
        for (j = 0; j < 15; j++) {
            ptyCharData[i].skills[j] = m;
        }
    }
    MemSet(ptyPartyData->bag, 0, 0x78);
    MemSet(ptyPartyData->keyItems, 0, 0x60);
    MemSet(ptyPartyData->storage, 0, 0x800);
    MemSet(ptyPartyData->itemFlags, 0, 0x800);
    MemSet(ptyPartyData->books, 0, 0x60);
    MemSet(ptyPartyData->skills, 0, 0x320);
    MemSet(ptyPartyData->eggs, 0, 0x2A0);
    MemSet(ptyPartyData->moveLevels, 0, 0x2D0);
    for (j = 0; j < 100; j++) {
        ptyPartyData->skills[j].owner = -1;
    }
    for (j = 0; j < 24; j++) {
        ptyPartyData->books[j].owner = -1;
    }
    for (j = 0; j < 24; j++) {
        ptyPartyData->eggs[j].owner = -1;
    }
}
