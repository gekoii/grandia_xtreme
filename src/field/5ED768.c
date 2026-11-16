#include "common.h"
#include "party.h"
#include "field/menu.h"

void UtilSortU64(u64 *, u32, void *);

void ItemSortAllItems(void) {
    u64 *list;
    s16 *tbl;
    u32 i;
    s32 k;

    list = itemWork->keys;
    tbl = ptyItemSortKeys;
    for (i = 0; i < 0x800; i++) {
        k = *tbl++;
        if (k == 0) {
            k = 0xFFFF;
        }
        list[i] = (s64)(k << 16) + (s32)i;
    }
    UtilSortU64(list, 0x800, (void *)0x70003000);
    for (i = 0; i < 0x800; i++) {
        itemWork->order[i] = list[i] & 0xFFFF;
    }
}
