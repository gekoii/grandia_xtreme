#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "sys.h"
#include "field/state.h"

void CamDebugPadControl(Camera *, Pad *);

void FldUpdate(s32 arg0) {
    s32 n;
    s32 i;
    s32 skip;
    Pad *pad;

    skip = 0;
    if (SysDemoGetMode() == 99) {
        FldMenuOpen(4);
    }
    if (SysDemoGetMode() == 9) {
        FldMenuOpen(11);
    }
    FldSetDrawContext(arg0, 0, 0);
    /* The nested do-while (0) wraps give n the original's allocation priority. */
    do {
        do {
            if (SysIsDebugMode() != 0) {
                n = 1;
                do {
                    do {
                        if (padPort0[0].held & 0x400) {
                            n = fldState.systemSpeed;
                        }
                    } while (0);
                } while (0);
            } else {
                n = 1;
            }
        } while (0);
    } while (0);
    pad = padPort0;
    FldPadApplyInputLock(pad);
    if (btlHandoffResult != 0) {
        FldBattleReturn();
    }
    if (SysIsDebugMode() != 0) {
        if (sysState.pauseMode == 0 || (pad[1].repeat & 0x80)) {
            skip = 1;
        }
        if (sysState.dbgMenuFlags & 0x20000000) {
            CamDebugPadControl(camField, &padPort1);
        }
    } else {
        skip = 1;
    }
    if (skip == 0) {
        GphScreenFxUpdate(fldScreenFx);
        GphScreenFxUpdate(&fldScreenFx[1]);
        return;
    }
    MapLoadUpdate();
    if (fldState.unk2D != 0) {
        n = 0;
    }
    for (i = 0; i < n; i++) {
        switch ((u32)fldState.mode) {
        case 0:
            FldUpdatePlay(arg0);
            break;
        case 1:
            FldUpdateMenu(arg0);
            break;
        case 4:
            FldUpdateNoControl(arg0);
            break;
        case 2:
            FldUpdatePlay(arg0);
            break;
        }
    }
    if (SysDemoGetMode() == 99 && EvtSystemFlagGet(0x8D) == 0) {
        SysDemoSetReady();
    }
    GphFadeUpdate(fldFades);
    GphFadeUpdate(&fldFades[1]);
    GphScreenFxUpdate(fldScreenFx);
    GphScreenFxUpdate(&fldScreenFx[1]);
    if (fldState.unk2D != 0) {
        if (sysState.unk11 == 0) {
            FldBattleStart();
            SysProgRequest(0, 3);
            fldState.unk2D = 0;
        }
    }
    if (fldState.pendingBattleStart != 0) {
        if (sysState.unk11 == 0) {
            fldState.pendingBattleStart = 0;
            fldState.unk2D = sysState.unk13;
            FldBattleStart();
            SysProgRequest(0, 3);
            GphScreenFxSet(fldScreenFx, 6, 0, 0, 0.0f);
            fldState.unk2D = 0;
        }
    }
}
