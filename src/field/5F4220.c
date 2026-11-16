/* rodata: 0x00674070-0x00674084 */
#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "field/n19.h"
#include "protos/field.h"

void ItemEggSpellRecFill(PtyManaEgg *, ItemEggSpellRec *, s16, s16);

void ItemEggSpellListBuild(s16 mode) {
    s32 i;
    s32 n;
    u16 id;

    for (i = 0; i < 24; i++) {
        itemWork->eggSpells[i].itemId = 0;
    }
    n = 0;
    ItemSortEggs();
    for (i = 0; i < 24; i++) {
        id = itemWork->order[i];
        if (ptyPartyData->eggs[id].itemId == 0) {
            continue;
        }
        switch (mode) {
        case 0:
        case 3:
            if (ptyPartyData->eggs[id].owner != -1 && PtyPartyFindChar(ptyPartyData->eggs[id].owner) == -1) {
                break;
            }
            ItemEggSpellRecFill(&ptyPartyData->eggs[id], &itemWork->eggSpells[n], mode, id);
            n++;
            break;
        case 1:
        case 2:
            if (PtyPartyFindChar(ptyPartyData->eggs[id].owner) == -1) {
                break;
            }
            if (ptyPartyData->eggs[id].owner == -1) {
                break;
            }
            ItemEggSpellRecFill(&ptyPartyData->eggs[id], &itemWork->eggSpells[n], mode, id);
            n++;
            break;
        case 4:
            if (ptyPartyData->eggs[id].owner != -1 && PtyCharIsJoined(ptyPartyData->eggs[id].owner) < 0) {
                break;
            }
            ItemEggSpellRecFill(&ptyPartyData->eggs[id], &itemWork->eggSpells[n], mode, id);
            n++;
            break;
        }
    }
    itemWork->unk6C6C = n;
}
