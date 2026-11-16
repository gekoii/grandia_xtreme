#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n23.h"

s32 FldMenuScreen1Update(void) {
    s32 moved;

    moved = 0;
    do { /* empty: scheduling barrier, the padPort0 load stays after the prologue */
    } while (0);
    if (!(padPort0[0].held & 0x1000)) {
    do { /* do-while(0): loop weight for moved (s1/s2) and the branch layout of the chain */
    if (padPort0[0].held & 0x4000) {
            if (fldMenuPartyPanel.row == 0) {
                fldMenuPartyPanel.row = 1;
                moved = 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        } else if (padPort0[0].held & 0x8000) {
            if (fldMenuPartyPanel.col != 0) {
                fldMenuPartyPanel.col = 0;
                moved = 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        } else if (padPort0[0].held & 0x2000) {
            if (fldMenuPartyPanel.col == 0) {
                fldMenuPartyPanel.col = 1;
                moved = 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        }
        } while (0);
    } else {
        if (fldMenuPartyPanel.row != 0) {
            fldMenuPartyPanel.row = 0;
            moved = 1;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
    }
    FldMenuCursorSetAt(fldMenuScreen1WinIds[fldMenuPartyPanel.row * 2 + fldMenuPartyPanel.col], 16.0f, 8.0f);
    if (moved) {
        FldMenuWinSetActive(fldMenuScreen1WinIds[fldMenuPartyPanel.row * 2 + fldMenuPartyPanel.col]);
    }
    return 0;
}
