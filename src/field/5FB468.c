#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "sys.h"


s32 ChrPlayerIsBusy(void) {
    ChrObj *pl;

    pl = ChrGetByNo(1);
    if (CamScrIsRunning() == 1) {
        return 1;
    }
    if (EvtScnrIsRunning() == 1) {
        return 1;
    }
    if (FldFlagListIsOpen() != 0) {
        return 1;
    }
    if (MiniGetState() != 0) {
        return 1;
    }
    if (WmapIsActive() != 0) {
        return 1;
    }
    if (EvtCharSelIsActive() != 0) {
        return 1;
    }
    if (pl == NULL) {
        return 1;
    }
    if (pl->moveCmds != 0) {
        return 1;
    }
    if (CamBlendIsActive() == 1) {
    one:
        return 1;
    }
    if (sysProgChangeReq == 1) {
        goto one; /* goto: shares the return 1 block (an || condition loses an insn) */
    }
    return 0;
}
