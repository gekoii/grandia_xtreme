#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/n19.h"

s32 ItemCanAdd(s16);

s32 PtyEquipRemove(s16 id, s16 slot) {
    s32 n;
    s16 item;

    n = PtyPartyGetSlot(id);
    item = ptyCharData[id].equip[itemEquipSlotOrder[slot]];
    if (ItemCanAdd(item) == -1) {
        return -1;
    }
    ptyCharData[id].equip[itemEquipSlotOrder[slot]] = 0;
    ItemAdd(item);
    PtyUnitCalcParams(&ptyPartyUnits[n], 0, 0);
    return 0;
}
