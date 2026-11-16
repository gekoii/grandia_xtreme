#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "party.h"

void ItemListsBuildAll(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (sysSaveData.party[i] != 0) {
            PtyCharStoreUnitStats(&ptyPartyUnits[i]);
        }
    }
    ItemBagListBuild(0);
    ItemEggListBuild(0);
    ItemBookListBuild(0);
    ItemSkillListBuild(0, -1);
    ItemValuableListBuild(0);
    ItemFlagGroupListBuild(0);
    ItemPartyStatusBuild(0);
    ItemEquipListBuild(0, 0);
}
