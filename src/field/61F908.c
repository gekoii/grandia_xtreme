#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/menu.h"



void FldMenuUpdate(void) {
    if (fldState.mode == 1) {
    if (fldMenuWork->openDelay != 0) {
        fldMenuWork->openDelay--;
        if (fldMenuWork->openDelay == 0) {
            GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 70.0f, 10.0f);
            GphScreenFxSet(fldScreenFx, 2, 1, 20, 3.0f);
            GphScreenFxSet(fldScreenFx, 6, 1, 0, 0.0f);
            SysScreenCaptureRequest();
            ItemListsBuildAll();
            MenuWidgetsInit();
            MenuHelpInit();
            ItemSetMenuType(fldMenuWork->openType);
            switch (fldMenuWork->openType) {
            case 0:
                FldMenuTopInit();
                FldMenuWinOpen(0x11, 0x170, 0x50, 0x1EA, 0x28, 0, 8, 0x2400B);
                fldMenuWork->unkB = 0;
                fldMenuWork->guideMode = 1;
                break;
            case 1:
                FldMenuPartyOrderInit();
                fldMenuWork->screen = 4;
                break;
            case 2:
                FldMenuSaveInit();
                fldMenuWork->screen = 9;
                break;
            case 3:
                FldMenuLoadInit();
                fldMenuWork->screen = 10;
                break;
            case 4:
                FldMenuMcCheckInit();
                fldMenuWork->screen = 11;
                break;
            case 5:
                FldMenuSkillmanInit();
                fldMenuWork->screen = 14;
                break;
            case 6:
                FldMenuShopInit(0);
                fldMenuWork->screen = 15;
                break;
            case 7:
                FldMenuShopInit(1);
                fldMenuWork->screen = 15;
                break;
            case 8:
                FldMenuShopInit(2);
                fldMenuWork->screen = 15;
                break;
            case 9:
                FldMenuShopInit(3);
                fldMenuWork->screen = 15;
                break;
            case 10:
                FldMenuJunctionInit();
                fldMenuWork->screen = 16;
                break;
            case 11:
                FldMenuGeoPointHelpInit();
                fldMenuWork->screen = 13;
                break;
            case 12:
                FldMenuJunctionRecipeInit();
                fldMenuWork->screen = 12;
                break;
            case 13:
                FldMenuOptionsInit();
                fldMenuWork->screen = 3;
                fldMenuWork->directOpen = 1;
                break;
            case 14:
                FldMenuLoadInit();
                fldMenuWork->screen = 10;
                fldMenuWork->directOpen = 1;
                break;
            }
        }
    } else {
        FldMenuCommandIconsUpdate(0x11);
        FldMenuScreenDispatch(fldMenuWork->screen);
    }
    }
}
