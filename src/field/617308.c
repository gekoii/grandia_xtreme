#include "common.h"
#include "field/state.h"

s16 FldSndGetMapSe(u8 arg0) {
    return fldSnd.mapSeBanks[arg0];
}
