#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemCanAdd(s16);

s32 PtyEquipRemoveUnusable(s16 id) {
    s32 i;
    s16 item;
    PtyEquipData *m;

    PtyPartyGetSlot(id);
    for (i = 0; i < 6; i++) {
        s32 v = ptyCharData[id].equip[i];
        m = ptyItemTable[v].equip;
        if (m != NULL && !((m->charMask >> id) & 1)) {
            item = v;
            if (ItemCanAdd(item) == -1) {
                ItemStorageAdd(item, 1);
            }
            ptyCharData[id].equip[i] = 0;
        }
    }
    return 0;
}
