#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/menu.h"
#include "field/n19.h"


s32 ItemEggListBuild(s16 mode) {
    s32 i;
    u16 idx;

    switch (mode) {
    case 1:
        itemWork->unk6ED4 = 0;
        break;
    case 0:
    case 2:
        itemWork->counts[1] = 0;
        break;
    }
    ItemSortEggs();
    for (i = 0; i < 24; i++) {
        idx = itemWork->order[i];
        if (ptyPartyData->eggs[idx].itemId != 0) {
            switch (mode) {
            case 0:
                if (ptyPartyData->eggs[idx].owner == -1 || PtyCharIsJoined(ptyPartyData->eggs[idx].owner) >= 0) {
                    ItemEggRecFill(&ptyPartyData->eggs[idx], &itemWork->eggs[itemWork->counts[1]], idx);
                    itemWork->counts[1]++;
                }
                break;
            case 1:
                if (ptyPartyData->eggs[idx].owner == -1) {
                    ItemEggRecFill(&ptyPartyData->eggs[idx], (ItemEggListRec *)&itemWork->magic[itemWork->unk6ED4], idx);
                    itemWork->unk6ED4++;
                }
                break;
            case 2:
                if (ptyPartyData->eggs[idx].owner != -1) {
                    ItemEggRecFill(&ptyPartyData->eggs[idx], &itemWork->eggs[itemWork->counts[1]], idx);
                    itemWork->counts[1]++;
                }
                break;
            }
        }
    }
    return 0;
}
