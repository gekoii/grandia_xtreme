#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveConfirmQuit(void) {
    s32 done;
    u16 k;

    done = 0;
    if (mcSaveStateChanged == 0) {
        k = mcSavePad->trigger;
        if (k & 0x20) {
            done = 1;
            McSaveSetState(0x800);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        } else if (k & 0x40) {
            done = 1;
            McSaveSetState(0);
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
        }
    }
    if (mcSaveStateChanged != 0) {
        /* s64 casts: these calls stay jal (no tail calls) */
        ((s64 (*)(s32))FldMenuMcDlgQuitNoSave)(0);
    } else if (done) {
        ((s64 (*)(s32))FldMenuMcDlgQuitNoSave)(2);
    } else {
        ((s64 (*)(s32))FldMenuMcDlgQuitNoSave)(1);
    }
}
