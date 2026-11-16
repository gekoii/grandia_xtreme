#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n23.h"

void *MemSet(void *, s32, s32);

void FldMenuOptionsInit(void) {
    MemSet(&fldMenuOptions, 0, sizeof(FldMenuOptions));
    fldMenuOptions.vibration = sysSaveData.optVibration;
    fldMenuOptions.camReverse = sysSaveData.optCamReverse;
    fldMenuOptions.soundMode = sysSaveData.optSoundMode;
    fldMenuOptions.ipStop = sysSaveData.optIpStop;
    fldMenuOptions.miniMapOff = sysSaveData.optMiniMapOff;
    fldMenuOptions.miniMap2A = sysSaveData.optMiniMap2A;
    fldMenuOptions.miniMap2B = sysSaveData.optMiniMap2B;
    FldMenuWinOpen(8, 0x140, 0xE0, 0x188, 0xC0, 0, 0, 0x88010);
    FldMenuWinSetActive(8);
    FldMenuGuideSet(7);
}
