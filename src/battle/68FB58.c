#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "battle/n28.h"
#include "pad.h"


void MenuTacticsAutoInput(void) {
    Pad *p = padPort0;
    s32 i;
    s32 k;
    s32 m1;
    f32 f;
    BtlUnit *u;
    BtlUnit *uu;
    BtlUnit *u3;
    MenuTacticsTag *ee;
    MenuTacticsTag *e;
    if (btlEnemyAliveCount <= 0) {
        if (menuAutoCamPanel.state == 1) {
            menuAutoCamPanel.state = 2;
        }
        return;
    }
    if (!(btlDebugCfg.flags & 0x10) && (p->trigger & 0x10)) {
        menuTacticsBannerTimer = 0x20;
        if (++btlAutoTactics >= 3) {
            btlAutoTactics = 1;
        }
        btlTacticsLabelsOn = 1;
        ee = menuTacticsTags;
        uu = btlUnits;
        m1 = -1;
        i = 0;
        for (k = 0; k < 4; ee++, uu++) {
            uu->ai = btlTactics[i][btlAutoTactics + m1];
            f = 1.0f;
            if (uu->ai == 0) {
                f = 0.0f;
            }
            ee->alpha = f;
            i++;
            k++;
        }

        u3 = btlUnits;
        for (k = 0; k < 4; k++, u3++) {
            if (u3->state >= 0x40 && u3->ai == 0) {
                break;
            }
        }
        if (k < 4) {
            btlCamParam.subMode = 0;
            if (btlCamParam.mode == 1) {
                BtlCamSetMode(1, 0x40, 0, 0);
            }
            menuAutoCamPanel.state = 2;
        }
    }
    if (btlAutoTactics != 0) {
        u = btlUnits;
        if (u[btlCamParam.unit].state < 2) {
            btlCamParam.unit = 0;
        }
        if (!(btlDebugCfg.flags & 0x10) && (p->trigger & 0x40)) {
            menuTacticsBannerTimer = 0x20;
            btlTacticsLabelsOn = 0;
            btlAutoTactics = 0;
            e = menuTacticsTags;
            for (i = 0; i < 4; i++) {
                e->alpha = 0.0f;
                e++;
            }
            ((s32 (*)(void))MenuAutoCamInput)(); /* cast: no tail call */
            return;
        }
        for (i = 0; i < 4; i++, u++) {
            if (u->state >= 0x40 && u->ai == 0) {
                break;
            }
        }
        if (i < 4) {
            return;
        }
        MenuAutoCamInput();
    }
}
