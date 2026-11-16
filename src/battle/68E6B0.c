#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "pad.h"


s32 MenuTacticsUpdate(void) {
    Pad *pad;
    s8 r;
    s32 i;
    s32 t;
    s32 t2;
    s32 b;
    s32 k;
    s32 j;
    s32 m1;
    s32 m2;
    u8 w;
    BtlUnit *u;
    BtlUnit *uc;
    MenuTacticsTag *e;
    if (menuTacticsState <= 0) {
        return 0;
    }
    pad = padPort0;
    k = pad->trigger;
    r = BtlMenuCursorStep(menuTacticsState == 1 ? k & 0xA00C : k & 0xC, menuTacticsSet, 1, 2);
    if (pad->trigger & 4) {
        MenuLrArrowsFlash(1);
    } else if (pad->trigger & 8) {
        MenuLrArrowsFlash(2);
    }
    if (r != menuTacticsSet) {
        if (menuTacticsState == 2) {
            btlUnits[menuTacticsMember].ai = menuTacticsLists[menuTacticsMember].sel;
        }
        u = btlUnits;
        m1 = -1;
        i = 0;
        for (j = 0; j < 4; j++) {
            btlTactics[i][menuTacticsSet + m1] = u->ai;
            i++;
            u++;
        }
        menuTacticsSet = r;
        u = btlUnits;
        for (j = 0; j < 4; j++) {
            w = btlTactics[j][menuTacticsSet - 1];
            u->ai = w;
            menuTacticsLists[j].sel = w;
            u++;
        }
        if (menuTacticsState == 1) {
            MenuCursorMoveTo((menuTacticsSet - 1) * 240.0f + 200.0f - 50.0f, 256.0f);
        } else {
            MenuTacticsCursorUpdate();
        }
    }
    if (menuTacticsState == 1) {
        if (pad->trigger & 0x60) {
            t = BtlMenuPadDecide(pad->trigger & 0x60);
            uc = btlUnits;
            e = menuTacticsTags;
            m2 = -1;
            j = 0;
            for (i = 0; i < 4; i++, uc++, e++) {
                menuTacticsLists[j].state = -1;
                if (uc->state >= 2) {
                    btlTactics[j][menuTacticsSet + m2] = uc->ai;
                    if (t == -1 || (s8)btlTactics[j][menuTacticsSet + m2] == 0) {
                        e->fade = -1;
                    }
                    e->slide = -1;
                }
                j++;
            }
            if (t == 1) {
                btlAutoTactics = menuTacticsSet;
                menuTacticsBannerTimer = 0x20;
            }
            menuTacticsState = 0;
            return t;
        }
        if (pad->trigger & 0x1000) {
            SndSePlay3D(0x7D1, 0, 0, 0);
            menuTacticsState = 2;
            menuTacticsLists[menuTacticsMember].sel = btlUnits[menuTacticsMember].ai;
            menuTacticsLists[menuTacticsMember].state = 0;
            MenuTacticsCursorUpdate();
        }
        return 0;
    }
    b = pad->repeat;
    if (b & 0xA000) {
        menuTacticsLists[menuTacticsMember].state = 0x10;
        btlUnits[menuTacticsMember].ai = menuTacticsLists[menuTacticsMember].sel;
        if (b & 0x8000) {
            SndSePlay3D(0x7D1, 0, 0, 0);
            do {
                if (--menuTacticsMember < 0) {
                    menuTacticsMember = 3;
                }
            } while (btlUnits[menuTacticsMember].state < 2);
        } else if (b & 0x2000) {
            SndSePlay3D(0x7D1, 0, 0, 0);
            do {
                if (++menuTacticsMember >= 4) {
                    menuTacticsMember = 0;
                }
            } while (btlUnits[menuTacticsMember].state < 2);
        }
        menuTacticsLists[menuTacticsMember].state = 0;
        menuTacticsLists[menuTacticsMember].sel = btlUnits[menuTacticsMember].ai;
        MenuTacticsCursorUpdate();
        return 0;
    }
    if (b & 0x5000) {
        menuTacticsLists[menuTacticsMember].sel = BtlMenuCursorStep(b & 0x5000, menuTacticsLists[menuTacticsMember].sel, 0, 7);
        MenuTacticsCursorUpdate();
    }
    if (pad->trigger & 0x60) {
        t2 = BtlMenuPadDecide(pad->trigger & 0x60);
        switch (t2) {
        case 1:
            btlUnits[menuTacticsMember].ai = menuTacticsLists[menuTacticsMember].sel;
            break;
        case -1:
            menuTacticsLists[menuTacticsMember].sel = btlUnits[menuTacticsMember].ai;
            break;
        }
        menuTacticsLists[menuTacticsMember].state = 0x10;
        menuTacticsState = 1;
        MenuCursorMoveTo((menuTacticsSet - 1) * 240.0f + 200.0f - 50.0f, 256.0f);
        MenuHelpClose();
    }
    return 0;
}
