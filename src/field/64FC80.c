#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallDraw(MiniCtl *w) {
    MiniUiResetTexCache();
    MiniPanelsDraw(0xBE, miniBallWork->game.panels, 0xA);
    if (SysIsDebugMode() != 0) {
        MiniDebugDrawInput();
        MiniBallDebugDraw();
    }
}
