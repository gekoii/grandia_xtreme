#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "protos/field.h"

void ItemInvCompareListBuild(void);

void ItemListBuild(s16 mode) {
    itemWork->gold = ptyPartyData->gold;
    switch (mode) {
    case 0:
        ItemBagListBuild(0);
        ItemBookListBuild(0);
        ItemValuableListBuild(0);
        ItemFlagGroupListBuild(0);
        ItemPartyStatusBuild(0);
        ItemEquipListBuild(0, 0);
        ItemEggChartBuild();
        switch (itemWork->menuType) {
        case 9:
            break;
        case 0:
            ItemEggListBuild(0);
            ItemSkillListBuild(0, -1);
            ItemEggSpellListBuild(1);
            break;
        case 8:
            ItemSkillListBuild(2, -1);
            break;
        case 10:
            ItemEggListBuild(0);
            ItemEggSpellListBuild(4);
            break;
        default:
            ItemEggListBuild(0);
            ItemEggSpellListBuild(0);
            break;
        }
        break;
    case 1:
        ItemBagListBuild(0);
        break;
    case 2:
        ItemEggListBuild(0);
        break;
    case 3:
        ItemBookListBuild(0);
        break;
    case 4:
        ItemSkillListBuild(0, -1);
        break;
    case 5:
        ItemValuableListBuild(0);
        break;
    case 6:
        ItemFlagGroupListBuild(0);
        break;
    case 7:
        ItemPartyStatusBuild(0);
        break;
    case 8:
        ItemInvCompareListBuild();
        break;
    }
    ItemUpdateLimits();
}
