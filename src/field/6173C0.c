#include "common.h"
#include "field/state.h"

void FldSndSetMapSe(u8 arg0, s16 arg1) {
    if (arg1 != -1) {
        fldSnd.mapSeBanks[arg0] = arg1;
    }
}
