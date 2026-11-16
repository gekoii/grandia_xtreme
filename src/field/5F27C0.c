#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/n19.h"


void PtyEquipSet(s16 id, s16 slot, s16 item) {
    s32 n;
    s16 *p;
    s16 old;

    n = PtyPartyGetSlot(id);
    p = (s16 *)(id * 0x84 + (u32)ptyCharData);
    p += 0x19;
    p += itemEquipSlotOrder[slot];
    old = *p;
    *p = item;
    ItemBagRemoveById(item);
    if (old != 0) {
        ItemAdd(old);
    }
    PtyUnitCalcParams(&ptyPartyUnits[n], 0, 0);
}
