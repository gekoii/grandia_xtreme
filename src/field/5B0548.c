#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/state.h"


void FldResume(void) {
    SysState *c;

    if (SysDemoGetMode() != 0) {
        fldState.mode = FLD_MODE_TRANSITION;
    }
    c = &sysState;
    if (c->unkE != 0) {
        MemHeapSelect(c->dataHeap);
        FldMapCtlInit();
        MapJumpRequest(c->nextMapNo, c->unk16);
        fldState.pendingBattleStart = 0;
        c->unkE = 0;
        switch (c->unkD) {
        case 1:
            GphFadeSetColor(fldFades, 0.0f, 0.0f, 0.0f, 128.0f);
            return;
        case 2:
            GphFadeSetColor(fldFades, 255.0f, 255.0f, 255.0f, 128.0f);
            return;
        case 3:
            GphScreenFxSet(fldScreenFx, 6, 1, 0, 0.0f);
            return;
        case 0:
        case 4:
            break;
        case 10:
            GphScreenFxSet(fldScreenFx, 6, 1, 0, 0.0f);
            fldState.pendingBattleStart = 1;
            break;
        }
    }
}
