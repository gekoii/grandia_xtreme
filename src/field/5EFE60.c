#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "party.h"


s16 ItemBagListBuild(s32 arg0) {
    PtyItem *ents;
    ItemBagRec *recs;
    s32 n;
    s32 i;
    u16 k;
    s16 id;
    s8 cnt;

    n = 0;
    ents = ptyItemTable;
    recs = itemWork->bag;
    ItemSortInventory();
    for (i = 0; i < 30; i++) {
        k = itemWork->order[i];
        id = ptyPartyData->bag[k].id;
        if (id != 0) {
            cnt = ptyPartyData->bag[k].count;
            if (cnt != 0 && ents[id].type != 14) {
                recs[n].unk6 = ItemGetUseClass(0, id);
                recs[n].count = cnt;
                recs[n].unk14 = ptyItemTable[id].price / 2;
                ItemInfoFill(&recs[n], id, k);
                n++;
            }
        }
    }
    itemWork->counts[0] = n;
    return n;
}
