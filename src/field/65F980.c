#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McLoadScanSlots(void) {
    s32 r;

    if (mcLoadStateChanged != 0) {
        r = McScanSlots(0);
    } else {
        r = McScanSlots(1);
    }
    if (r == 1) {
        McLoadSetState(2);
    }
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcLoadStateChanged != 0) {
            FldMenuMcDlgReading(0);
        } else if (r == 1) {
            FldMenuMcDlgReading(2);
        } else {
            FldMenuMcDlgReading(1);
        }
    } while (0);
}
