#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McLoadCardOutMsg(void) {
    s32 done;

    done = 0;
    if (McIsBusy() == 0) {
        if (mcLoadPad->trigger != 0) {
            done = 1;
            McLoadSetState(0);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        }
    }
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcLoadStateChanged != 0) {
            FldMenuMcDlgCardRemoved(0);
        } else if (done) {
            FldMenuMcDlgCardRemoved(2);
        } else {
            FldMenuMcDlgCardRemoved(1);
        }
    } while (0);
}
