#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/menu.h"
#include "field/n19.h"


s32 ItemBookListBuild(s16 mode) {
    s32 i;
    u16 k;

    switch (mode) {
    case 0:
        itemWork->counts[2] = 0;
        break;
    case 1:
        itemWork->books3Count = 0;
        break;
    }
    ItemSortBooks();
    for (i = 0; i < 24; i++) {
        k = itemWork->order[i];
        if (ptyPartyData->books[k].itemId != 0) {
            switch (mode) {
            case 0:
                if (ptyPartyData->books[k].owner == -1 || PtyCharIsJoined(ptyPartyData->books[k].owner) >= 0) {
                    ItemBookRecFill(&ptyPartyData->books[k], (ItemBookRec *)&itemWork->books[itemWork->counts[2]], k);
                    itemWork->counts[2]++;
                }
                break;
            case 1:
                if (ptyPartyData->books[k].owner == -1) {
                    ItemBookRecFill(&ptyPartyData->books[k], (ItemBookRec *)&itemWork->books3[itemWork->books3Count], k);
                    itemWork->books3Count++;
                }
                break;
            }
        }
    }
    return 0;
}
