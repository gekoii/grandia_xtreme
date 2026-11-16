#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveAskFormat(void) {
    s32 hit;
    u16 btn;

    hit = 0;
    if (mcSaveStateChanged == 0) {
        btn = mcSavePad->trigger;
        if (btn & 0x20) {
            hit = 1;
            McSaveSetState(4);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        } else if (btn & 0x40) {
            hit = 1;
            McSaveSetState(0);
            SndSePlay(0x7D4, 0, 0, 0x1E, 0);
        }
    }
    if (mcSaveStateChanged != 0) {
        /* s64 casts: these calls stay jal (no tail calls) */
        ((s64 (*)(s32))FldMenuMcDlgFormatPrompt)(0);
    } else if (hit != 0) {
        ((s64 (*)(s32))FldMenuMcDlgFormatPrompt)(2);
    } else {
        ((s64 (*)(s32))FldMenuMcDlgFormatPrompt)(1);
    }
}
