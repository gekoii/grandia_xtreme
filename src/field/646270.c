/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/n24.h"


s32 MiniConcertUpdate(void) {
    s32 r;
    u32 n;
    s32 st;

    MiniInputUpdate();
    if (miniConcertWork->unk8CB0 < 0x100) {
        if (MiniGetUi()->input.trigger != 0) {
            if (miniConcertWork->state == 4 && miniConcertWork->subState == 3) {
                n = miniConcertWork->unk8CB0;
                miniConcertWork->unk8AAE[n] = miniConcertWork->frame - miniConcertWork->unk16;
                miniConcertWork->unk8CB0 = n + 1;
            }
        }
    }
    switch (miniConcertWork->state) {
    case 1:
        if (EvtScnrIsRunning() == 0) {
            if (SysIsDebugMode() != 0) {
                r = EvtGlobalFlagGet(0x5F0);
                if (r == 1) {
                    miniConcertWork->unk35 = VuRandInt(5);
                    if (EvtGlobalFlagGet(0x5F1) == r) {
                        miniConcertWork->unk35 = sysSaveData.unk1F6D;
                    }
                    miniConcertWork->state = 0;
                    return 1;
                }
            }
            if (MiniGetKind() == 0) {
                MiniConcertSetupPreset();
                miniConcertWork->state = 4;
                miniConcertWork->subState = 1;
                MenuWidgetsInit();
            } else {
                EvtGlobalFlagSet(0x40F, 1);
                MiniConcertSetupSelect();
                miniConcertWork->state = 2;
                miniConcertWork->subState = 1;
                MenuWidgetsInit();
            }
        }
        break;
    case 2:
        MiniConcertSongSelectUpdate();
        break;
    case 3:
        MiniConcertStageSetupUpdate();
        break;
    case 4:
        MiniConcertPlayUpdate();
        miniConcertWork->unkA0 += 0.005f;
        if (miniConcertWork->unkA0 > 6.28318f) {
            miniConcertWork->unkA0 -= 6.28318f;
        }
        break;
    case 5:
        MiniConcertRankCalc();
        miniConcertWork->state = 0;
        return 1;
    }
    st = miniConcertWork->state;
    if (st < 5) {
        if (st >= 2) {
            MiniPanelsUpdate(miniConcertWork->menu.panels, 2);
            MiniPanelsUpdate(miniConcertWork->stage.panels, 4);
            MiniPanelsUpdate(miniConcertWork->panels, 4);
            miniConcertWork->frame++;
        }
    }
    return 0;
}
