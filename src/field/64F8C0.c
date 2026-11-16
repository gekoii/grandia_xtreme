#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


s32 MiniBallUpdate(void) {
    s32 s;

    MiniInputUpdate();
    switch (miniBallWork->state) {
    case 1:
        if (EvtScnrIsRunning() == 0) {
            EvtGlobalFlagSet(0x40F, 1);
            miniBallWork->state = 2;
            miniBallWork->subState = 1;
        }
        break;
    case 2:
        MiniBallEntryUpdate();
        break;
    case 3:
        MiniBallSetupUpdate();
        break;
    case 4:
        MiniBallPlayUpdate();
        break;
    case 5:
        EvtGlobalFlagSet(0x40F, 0);
        if (SysIsDebugMode() != 0 && EvtGlobalFlagGet(0x5F8) == 1) {
            miniBallWork->game.gold = 9999999;
        }
        PtySetGold(miniBallWork->game.gold);
        MenuCursorShow(0.0f, 0.0f);
        MenuCursorShow(0.0f, 0.0f);
        MenuCursorHide();
        miniBallWork->state = 0;
        return 1;
    }
    s = miniBallWork->state;
    if (s < 5) {
        if (s >= 2) {
            MiniBallGoldDisplayUpdate();
            MiniSpritesUpdate(miniBallWork->game.sprites, 0x3F);
            MiniPanelsUpdate(miniBallWork->game.panels, 10);
        }
    }
    miniBallWork->frame++;
    return 0;
}
