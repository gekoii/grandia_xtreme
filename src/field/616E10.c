#include "common.h"
#include "field/state.h"
#include "field/n22.h"

void FldSndReset(void) {
    fldSnd.bgmId = 0xFFFFFFFF;
    fldSnd.bgmHandle = 0xFFFFFFFF;
    fldSnd.strmHandle = 0xFFFFFFFF;
    fldSnd.strmId = 0xFFFFFFFF;
    fldSnd.mapSeBanks[0] = -1;
    fldSnd.mapSeBanks[1] = -1;
    fldSnd.mapSeBanks[2] = -1;
    fldSnd.mapSeBanks[3] = -1;
    fldSnd.partySeBanks[0] = 0xFFFF;
    fldSnd.partySeBanks[1] = 0xFFFF;
    fldSnd.partySeBanks[2] = 0xFFFF;
    fldSnd.distScale = 6000.0f;
    fldSndStreamFrames = 0;
}
