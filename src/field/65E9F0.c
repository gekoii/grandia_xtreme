#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveFormatFailedMsg(void) {
    s32 done;

    done = 0;
    if (mcSaveStateChanged == 0) {
        if (mcSavePad->trigger != 0) {
            McSaveSetState(0);
            done = 1;
        }
    }
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcSaveStateChanged != 0) {
            FldMenuMcDlgFormatFailed(0);
        } else if (done) {
            FldMenuMcDlgFormatFailed(2);
        } else {
            FldMenuMcDlgFormatFailed(1);
        }
    } while (0);
}
