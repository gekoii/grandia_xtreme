#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void BtlHudDraw(void) {
    if (!(btlHudHideFlags & 0x1000)) {
        MenuTacticsDrawTags();
        if (!(btlHudHideFlags & 2)) {
            IpPanelDraw();
        }
        if (!(btlHudHideFlags & 1)) {
            IpGaugeDraw();
        }
        if (!(btlHudHideFlags & 4)) {
            BtlHudDmgDraw();
        }
    }
}
