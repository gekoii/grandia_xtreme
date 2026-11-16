#include "common.h"
#include "party.h"
#include "field/menu.h"

void UtilSortU64(u64 *, u32, void *);

void ItemSortSkills(void) {
    u64 *keys;
    u32 i;
    s32 k;
    s32 id;

    keys = itemWork->keys;
    for (i = 0; i < 100; i++) {
        id = ptyPartyData->skills[i].id;
        k = 0xFFFF;
        if (id != 0) {
            k = id;
        }
        keys[i] = (s64)(k << 16) + (s32)i;
    }
    UtilSortU64(keys, 100, (void *)0x70003000);
    for (i = 0; i < 100; i++) {
        itemWork->order[i] = keys[i] & 0xFFFF;
    }
}
