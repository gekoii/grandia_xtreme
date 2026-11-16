#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "pad.h"


void MenuAutoCamInput(void) {
    Pad *p = padPort0;
    if (btlAutoTactics == 0) {
        btlCamParam.subMode = 0;
        if (btlCamParam.mode == 1) {
            BtlCamSetMode(1, 0x40, 0, 0);
        }
        menuAutoCamPanel.state = 2;
        return;
    }
    if (!(btlDebugCfg.flags & 0x10)) {
        if (p->trigger & 0x80) {
            if (++btlCamParam.subMode >= 4) {
                btlCamParam.subMode = 0;
            }
            if (btlCamParam.mode == 1) {
                BtlCamSetMode(1, 0x40, 0, 0);
            }
        } else if (btlCamParam.subMode >= 2 && (p->trigger & 0xC)) {
            if (p->trigger & 4) {
                do {
                    if (btlCamParam.unit > 0) {
                        btlCamParam.unit--;
                    } else {
                        btlCamParam.unit = 3;
                    }
                } while (btlUnits[btlCamParam.unit].state < 2);
            } else {
                do {
                    if (btlCamParam.unit < 3) {
                        btlCamParam.unit++;
                    } else {
                        btlCamParam.unit = 0;
                    }
                } while (btlUnits[btlCamParam.unit].state < 2);
            }
        } else if (btlCamParam.subMode == 3 && (p->trigger & 3)) {
            if (p->trigger & 1) {
                btlCamParam.unk4 -= 0x2000;
            } else {
                btlCamParam.unk4 += 0x2000;
            }
            btlCamParam.unk4 = (u16)btlCamParam.unk4;
        }
    }
    menuAutoCamPanel.state = 1;
    menuAutoCamPanel.mode = btlCamParam.subMode;
}
