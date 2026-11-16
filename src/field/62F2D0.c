#include "common.h"
#include "save.h"
#include "protos/field.h"




s32 FldMenuMagicAnyCanUse(s32 arg0, s32 arg1) {
    s32 i;
    s32 id;

    for (i = 0; i < 4; i++) {
        id = sysSaveData.party[i];
        if (id != 0) {
            if (ItemEggSpellCanCast(--id, arg0, arg1) == 0) {
                return 1;
            }
        }
    }
    return 0;
}
