#include "common.h"
#include "party.h"
#include "field/menu.h"

void UtilSortU64(u64 *, u32, void *);

void ItemSortEggs(void) {
    u64 *keys;
    u32 i;
    s32 k;

    keys = itemWork->keys;
    for (i = 0; i < 24; i++) {
        if (ptyPartyData->eggs[i].itemId != 0) {
            k = ptyItemSortKeys[ptyPartyData->eggs[i].itemId];
        } else {
            k = 0xFFFF;
        }
        keys[i] = (s64)(k << 16) + (s32)i;
    }
    UtilSortU64(keys, 24, (void *)0x70003000);
    for (i = 0; i < 24; i++) {
        itemWork->order[i] = keys[i] & 0xFFFF;
    }
}
