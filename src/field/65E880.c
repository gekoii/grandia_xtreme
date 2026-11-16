#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveNoSpaceMsg(void) {
    s32 done;

    done = 0;
    if (mcSaveStateChanged == 0) {
        if (McIsBusy() == 0) {
            if (mcSavePad->trigger != 0) {
                done = 1;
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                McSaveSetState(0);
            }
        }
    }
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcSaveStateChanged != 0) {
            FldMenuMcDlgNoSpace(0);
        } else if (done) {
            FldMenuMcDlgNoSpace(2);
        } else {
            FldMenuMcDlgNoSpace(1);
        }
    } while (0);
}
