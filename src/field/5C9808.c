#include "common.h"
#include "save.h"
#include "protos/field.h"

typedef struct {
    u16 a, b, c, d;
} Quad;



void FldLoadGameRestore(void) {
    Quad q;

    ItemPartyInitMode(1);
    q.a = sysSaveData.party[0];
    q.b = sysSaveData.party[1];
    q.c = sysSaveData.party[2];
    q.d = sysSaveData.party[3];
    PtyPartySet(&q);
    if (EvtSystemFlagGet(0xFE) == 1) {
        SysSetDebugMode();
    }
    if (sysSaveData.optVibration == 0) {
        PadSetVibEnabled(1);
    } else {
        PadSetVibEnabled(0);
    }
    SndSetOutputMode(sysSaveData.optSoundMode);
}
