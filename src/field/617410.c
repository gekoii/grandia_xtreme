#include "common.h"
#include "field/state.h"

void FldSndSetPartySe(s8 arg0, u16 arg1) {
    if (arg1 != 0) {
        fldSnd.partySeBanks[arg0] = arg1;
    }
}
