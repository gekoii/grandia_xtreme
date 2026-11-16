#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveConfirmOverwrite(void) {
    s32 done;
    u16 k;

    done = 0;
    if (mcSaveStateChanged == 0) {
        k = mcSavePad->trigger;
        if (k & 0x20) {
            done = 1;
            McSaveSetState(3);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        } else if (k & 0x40) {
            done = 2;
            McSaveSetState(2);
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
        }
    }
    if (mcSaveStateChanged != 0) {
        /* s64 casts: these calls stay jal (no tail calls) */
        ((s64 (*)(s32))FldMenuMcDlgOverwrite)(0);
        return;
    }
    if (done == 1) {
        ((s64 (*)(s32))FldMenuMcDlgOverwrite)(2);
    } else if (done == 2) {
        ((s64 (*)(s32))FldMenuMcDlgOverwrite)(2);
    } else {
        FldMenuMcDlgOverwrite(1);
    }
}
