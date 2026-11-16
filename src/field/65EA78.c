#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveWriteFailedMsg(void) {
    s32 done;

    done = 0;
    if (mcSaveStateChanged == 0) {
        if (McIsBusy() != 0) {
            return;
        }
        if (mcSavePad->trigger != 0) {
            McSaveSetState(0);
            done = 1;
        }
    }
    if (mcSaveStateChanged != 0) {
        FldMenuMcDlgSaveFailed(0);
    } else if (done) {
        FldMenuMcDlgSaveFailed(2);
    } else {
        FldMenuMcDlgSaveFailed(1);
    }
}
