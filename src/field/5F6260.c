#include "common.h"
#include "protos/field.h"
#include "party.h"

s32 ItemCanAdd(s16);

s16 ItemCharDropUnusableEquip(s16 a) {
    s32 ret;
    s32 i;
    s32 id;
    PtyEquipData *m;

    ret = ItemCharHasEquipAllSkill(a);
    if (ret == 0) {
    for (i = 1; i < 6; i++) {
        id = ptyCharData[a].equip[i];
        if (id != -1) {
            m = ptyItemTable[id].equip;
            if (m != NULL && !((m->charMask >> a) & 1)) {
                if (ItemCanAdd(id) == -1) {
                    ItemStorageAdd(id, 1);
                } else {
                    ItemAdd(id);
                }
                ptyCharData[a].equip[i] = 0;
            }
        }
    }
    }
    return ret;
}
