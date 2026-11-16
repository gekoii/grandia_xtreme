#include "common.h"
#include "party.h"
#include "field/menu.h"

s16 ItemValuableListBuild(s32 arg0) {
    s32 i;
    s32 n;
    s16 id;
    ItemValuable *e;
    PtyItem *d;
    s8 c;
    u8 b;
    s32 v;

    n = 0;
    e = itemWork->valuables;
    for (i = 0; i < 24; i++) {
        id = ptyPartyData->keyItems[i].id;
        if (id != 0) {
            d = (PtyItem *)((id << 5) + (u32)ptyItemTable);
            c = ptyPartyData->keyItems[i].count;
            if (d->type == PTY_ITEM_KEY) {
                b = d->icon;
                v = d->name;
                n++;
                e->id = id;
                e->unk2 = b;
                e->unk3 = c;
                e->unk4 = v;
                e->def = d;
                e++;
            }
        }
    }
    itemWork->counts[4] = n;
    return n;
}
