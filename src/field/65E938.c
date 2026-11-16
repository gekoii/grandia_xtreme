#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveNoSpaceMsg2(void) {
    s32 done;

    done = 0;
    if (mcSaveStateChanged == 0) {
        if (McIsBusy() != 0) {
            return;
        }
        if (mcSavePad->trigger != 0) {
            done = 1;
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            McSaveSetState(0);
        }
    }
    if (mcSaveStateChanged != 0) {
        FldMenuMcDlgNoSpace(0);
    } else if (done) {
        FldMenuMcDlgNoSpace(2);
    } else {
        FldMenuMcDlgNoSpace(1);
    }
}
