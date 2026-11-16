#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "protos/field.h"

s32 ItemEggFuseCommit(s16 a, s16 b) {
    s16 n;
    s32 t;

    ItemEggDiscard(a + 1000);
    ItemEggDiscard(b + 1000);
    n = ItemEggFindFreeSlot();
    if (n == -1) {
        return -1;
    }
    ((PtyPartyData *)(n * 0x1C + (u32)ptyPartyData))->eggs[0] = itemWork->u6E20.eggRecs[0];
    t = itemWork->u6E20.eggRecs[0].itemId;
    ItemEggChartSetKnown(t - 0x12C);
    ItemEggListBuild(0);
    return 0;
}
