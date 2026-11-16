#include "common.h"
#include "party.h"
#include "save.h"
#include "sys.h"
#include "protos/field.h"

typedef struct {
    u8 a, b, c, pad;
    u8 d, e, f, g;
} Save;

void MemSet(void *, s32, s32);

void FldNewGameReset(void) {
    Save s;

    s.a = sysSaveData.optVibration;
    s.b = sysSaveData.optCamReverse;
    s.c = sysSaveData.optSoundMode;
    s.d = sysSaveData.optIpStop;
    s.e = sysSaveData.optMiniMapOff;
    s.f = sysSaveData.optMiniMap2A;
    s.g = sysSaveData.optMiniMap2B;
    MemSet(ptyBattleParams, 0, 0x2A0);
    MemSet(ptyPartyUnits, 0, 0x3180);
    MemSet(ptyCharCalcUnits, 0, 0x18C0);
    MemSet(&btlHandoff, 0, 0x140);
    MemSet(&sysSaveData, 0, 0x3300);
    sysSaveData.optVibration = s.a;
    sysSaveData.optCamReverse = s.b;
    sysSaveData.optSoundMode = s.c;
    sysSaveData.optIpStop = s.d;
    sysSaveData.optMiniMapOff = s.e;
    sysSaveData.optMiniMap2A = s.f;
    sysSaveData.optMiniMap2B = s.g;
    EvtSystemFlagSet(1, 1);
    EvtSystemFlagSet(0xB, 1);
    sysSaveData.party[0] = 1;
    sysSaveData.mainFlag = 10000;
    ItemPartyInitMode(0);
    sysLoopFrameCount = 0;
    FldPlayTimeSet(0);
    if (sysSaveData.optVibration == 0) {
        PadSetVibEnabled(1);
    } else {
        PadSetVibEnabled(0);
    }
    SndSetOutputMode(sysSaveData.optSoundMode);
}
