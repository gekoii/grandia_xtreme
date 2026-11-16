#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n21.h"


s32 FldThreatGaugeIsHidden(void) {
    s32 st;
    ChrObj *c;

    st = 0;
    if (FldMapIsNoKamae() == 1) {
        st = 1;
    }
    if (MapLoadGetState() != -1) {
        st = 2;
    }
    if (ChrPlayerIsBusy() == 1) {
        st = 4;
    }
    if (camCtl.aiType != 0) {
        st = 5;
    }
    if (fldState.mode != 0) {
        st = 6;
    }
    if (MiniGetState() != 0) {
        st = 8;
    }
    if (ChrPartyCount() == 0) {
        st = 1;
    }
    c = ChrGetByNo(1);
    if (c == NULL) {
        st = 9;
    } else if (c->moveCmds != 0) {
        st = 10;
    }
    if (st != 0) {
        fldThreatGauge->showDelay = 0;
        fldThreatGauge->fadeIn = 0;
        return 1;
    }
    if (fldThreatGauge->showDelay < 60) {
        fldThreatGauge->showDelay++;
        return 1;
    }
    return 0;
}
