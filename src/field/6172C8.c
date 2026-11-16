#include "common.h"
#include "field/state.h"

s32 FldSndMapSeIsLoaded(u8 arg0, s16 arg1) {
    if (arg1 == -1) {
        return 1;
    }
    return fldSnd.mapSeBanks[arg0] == arg1;
}
