#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/state.h"
#include "field/menu.h"

void MemSet(void *, s32, s32);

void FldMenuOpen(u8 arg0) {
    s16 ids[4];
    ChrObj *info;
    s32 i;

    if (GphScreenFxGetFlags(&fldScreenFx[0]) & 0x400) {
        return;
    }
    if (GphScreenFxGetFlags(&fldScreenFx[0]) & 0x10) {
        return;
    }
    if (fldState.mode == 1) {
        return;
    }
    if (MapLoadGetState() < 10 && MapLoadGetState() != -1) {
        return;
    }
    info = ChrGetByNo(1);
    if (info != NULL && info->stunState != 0) {
        return;
    }
    MemSet(fldMenuWork, 0, 0xC6B0);
    for (i = 0; i < 4; i++) {
        ids[i] = sysSaveData.party[i];
    }
    PtyPartySet(ids);
    PtyPartyGet(ids);
    for (i = 0; i < 4; i++) {
        sysSaveData.party[i] = ids[i];
    }
    fldState.mode = 1;
    EvtSystemFlagSet(0x8D, 1);
    FldMenuWinClose(0, 0, 0);
    FldMenuWinClose(1, 0, 0);
    fldMenuWork->openDelay = 2;
    fldMenuWork->openType = arg0;
    if (arg0 != 4) {
        SndSePlay(0x7DD, 0, 0, 0x1E, 0);
    }
}
