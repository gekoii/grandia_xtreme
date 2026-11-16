/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "protos/battle.h"

f32 VuSinAngle(s32);

void MenuTacticsAnimUpdate(void) {
    s32 i;
    s32 j;
    MenuTacticsTag *e;
    MenuTacticsList *p;
    f32 f;
    if (btlPhase != 0x80 && btlPhase != 1) {
        return;
    }
    if (btlEncounterFlags < 0x21) {
        if (btlActingUnit >= 4 || btlUnits[btlActingUnit].state != 0x81) {
            MenuTacticsAutoInput();
        }
    }
    if (menuAutoCamPanel.state != 0) {
        MenuAutoCamPanelUpdate();
    }
    if (menuTacticsBannerTimer > 0) {
        menuTacticsBannerTimer--;
    }
    e = menuTacticsTags;
    for (i = 0; i < 4; i++, e++) {
        if (btlUnits[i].state < 2) {
            continue;
        }
        if (e->slide != 0) {
            switch (e->slide) {
            case -1:
                e->slide--;
                e->yFrom = e->y;
                e->timer = 0;
            case -2:
                if (e->timer < 16) {
                    f = VuSinAngle(e->timer * 1024.0f);
                    e->y = (58.0f - e->yFrom) * f + e->yFrom;
                } else {
                    e->y = 58.0f;
                    e->slide = 0;
                }
                break;
            case 1:
                e->slide++;
                e->yFrom = e->y;
                e->timer = 0;
            case 2:
                if (e->timer < 16) {
                    f = VuSinAngle(e->timer * 1024.0f);
                    e->y = (126.0f - e->yFrom) * f + e->yFrom;
                } else {
                    e->y = 126.0f;
                    e->slide = 0;
                }
                break;
            }
            e->timer++;
        }
        if (e->fade != 0) {
            if (e->fade > 0) {
                e->alpha += 0.0625f;
                if (e->alpha > 1.0f) {
                    e->alpha = 1.0f;
                    e->fade = 0;
                }
            } else {
                e->alpha -= 0.0625f;
                if (e->alpha < 0.0f) {
                    e->alpha = 0.0f;
                    e->fade = 0;
                }
            }
        }
    }
    if (menuTacticsState > 0) {
        if (menuTacticsOpenTimer > 0) {
            menuTacticsOpenTimer--;
        }
        p = menuTacticsLists;
        for (j = 0; j < 4; j++, p++) {
            if (p->state < 0) {
                continue;
            }
            switch (p->state) {
            case 0:
                p->alpha += 0.0625f;
                if (p->alpha >= 1.0f) {
                    p->alpha = 1.0f;
                }
                break;
            case 16:
                p->alpha -= 0.0625f;
                if (p->alpha <= 0.0f) {
                    p->state = -1;
                }
                break;
            }
        }
    }
}
