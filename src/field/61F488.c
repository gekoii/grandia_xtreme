#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/menu.h"

s32 FldMenuItemUpdate(void);
s32 FldMenuMagicUpdate(void);
s32 FldMenuJunctionUpdate(void);

void FldMenuScreenDispatch(s32 arg0) {
    s32 next;

    next = -1;
    ItemListBuild(0);
    switch (arg0) {
    case 0:
        next = FldMenuTopUpdate(0x11);
        break;
    case 1:
        next = FldMenuScreen1Update();
        break;
    case 2:
        next = FldMenuItemUpdate();
        break;
    case 3:
        next = FldMenuOptionsUpdate();
        break;
    case 4:
        next = FldMenuPartyOrderUpdate();
        break;
    case 5:
        next = FldMenuTacticsUpdate();
        break;
    case 6:
        next = FldMenuEquipUpdate();
        break;
    case 7:
        next = FldMenuMagicUpdate();
        break;
    case 8:
        next = FldMenuStatusUpdate();
        break;
    case 9:
        next = FldMenuSaveUpdate();
        break;
    case 10:
        next = FldMenuLoadUpdate();
        break;
    case 11:
        next = FldMenuMcCheckUpdate();
        break;
    case 12:
        next = FldMenuJunctionRecipeUpdate();
        break;
    case 13:
        next = FldMenuGeoPointHelpUpdate();
        break;
    case 14:
        next = FldMenuSkillmanUpdate();
        break;
    case 15:
        next = FldMenuShopUpdate();
        break;
    case 16:
        next = FldMenuJunctionUpdate();
        break;
    case 0xFF:
        if (FldMenuWinAllClosed() == 1) {
            FldTreasureRefreshNames();
            GphScreenFxSet(fldScreenFx, 6, 0, 0, 0.0f);
            EvtSystemFlagSet(0x8D, 0);
            fldMenuWork->screen = 0;
            if (SysDemoGetMode() != 0 || fldMenuWork->openType == 14) {
                fldState.mode = 4;
            } else {
                fldState.mode = 0;
            }
        }
        break;
    }
    FldMenuGuideUpdate(1);
    if (next == -1) {
        return;
    }
    if (next == arg0) {
        return;
    }
    MenuHelpClose();
    FldMenuTopExit();
    FldMenuItemExit();
    FldMenuOptionsExit();
    FldMenuTacticsExit();
    FldMenuEquipExit();
    FldMenuMagicExit();
    FldMenuStatusExit();
    FldMenuSaveExit();
    FldMenuLoadExit();
    FldMenuMcCheckExit();
    FldMenuJunctionRecipeExit();
    FldMenuGeoPointHelpExit();
    FldMenuSkillmanExit();
    FldMenuShopExit();
    FldMenuJunctionExit();
    FldMenuWinLrArrowsSet(0, 0, 0.0f);
    switch (next) {
    case 0:
    case 1:
        FldMenuTopInit();
        break;
    case 2:
        FldMenuItemInit();
        break;
    case 3:
        FldMenuOptionsInit();
        break;
    case 4:
        FldMenuPartyOrderInit();
        break;
    case 5:
        FldMenuTacticsInit();
        break;
    case 6:
        FldMenuEquipInit();
        break;
    case 7:
        FldMenuMagicInit();
        break;
    case 8:
        FldMenuStatusInit(fldMenuWork->unk18 * 2 + fldMenuWork->unk1C);
        break;
    case 9:
        FldMenuSaveInit();
        break;
    case 10:
        FldMenuLoadInit();
        break;
    case 11:
        FldMenuMcCheckInit();
        break;
    case 12:
        FldMenuJunctionRecipeInit();
        break;
    case 13:
        FldMenuGeoPointHelpInit();
        break;
    case 14:
        FldMenuSkillmanInit();
        break;
    case 16:
        FldMenuJunctionInit();
        break;
    }
    fldMenuWork->screen = next;
}
