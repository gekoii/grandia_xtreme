#include "common.h"
#include "party.h"
#include "field/menu.h"

void UtilSortU64(u64 *, u32, void *);

void ItemSortValuables(void) {
    u64 *keys;
    u32 i;
    s32 k;

    keys = itemWork->keys;
    for (i = 0; i < 24; i++) {
        if (ptyPartyData->keyItems[i].id != 0) {
            k = ptyItemSortKeys[ptyPartyData->keyItems[i].id];
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
