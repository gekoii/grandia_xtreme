#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "protos/battle.h"

void MenuTacticsOpen(u8 *arg) {
    s32 i;
    s32 k;
    s32 m1;
    MenuTacticsTag *e;
    BtlUnit *u;
    menuTacticsState = 1;
    menuTacticsOpenTimer = 8;
    menuTacticsMember = *arg;
    menuTacticsSet = btlAutoTactics ? btlAutoTactics : 1;
    e = menuTacticsTags;
    u = btlUnits;
    i = 0;
    m1 = -1;
    for (k = 0; k < 4; k++) {
        if (u->state >= 2) {
            e->fade = 1;
            u->ai = btlTactics[i][menuTacticsSet + m1];
            e->slide = 1;
        }
        i++;
        u++;
        e++;
    }
    btlTacticsLabelsOn = 1;
    btlAutoTactics = 0;
    for (i = 0; i < 4; i++) {
        menuTacticsLists[i].state = -1;
        menuTacticsLists[i].alpha = 0;
    }
    MenuCursorShow((menuTacticsSet - 1) * 240.0f + 200.0f - 50.0f, 256.0f);
}
