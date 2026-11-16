#include "common.h"
#include "pad.h"
#include "protos/field.h"




s32 FldMenuPartySelect(u8 base, s32 *a, s32 *b) {
    s32 n;

    n = ChrPartyCount();
    if (n == 1) {
        *b = 0;
        *a = 0;
    }
    if (n < 2) {
        *a = 0;
    }
    if (n >= 3) {
        if (padPort0[0].held & 0x1000) {
            if (*a != 0) {
                *a = 0;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        }
        if (n != 3 || *b != 1) {
            if (padPort0[0].held & 0x4000) {
                if (*a == 0) {
                    *a = 1;
                    SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                }
            }
        }
    }
    if (n >= 2 && (n != 3 || *a != 1)) {
        if (padPort0[0].held & 0x8000) {
            if (*b != 0) {
                *b = 0;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        }
        if (padPort0[0].held & 0x2000) {
            if (*b == 0) {
                *b = 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        }
    }
    if (n == 1) {
        *b = 0;
        *a = 0;
    }
    if (n < 2) {
        *a = 0;
    }
    FldMenuCursorSetAt(base + *a * 2 + *b, 16.0f, 8.0f);
    FldMenuWinSetActive(base + *a * 2 + *b);
    return 0;
}
