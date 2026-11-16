#include "common.h"
#include "field/state.h"

u16 FldSndGetStepSe(u8 arg0) {
    if (arg0 >= 4) {
        arg0 = 0;
    }
    return fldSnd.stepSe[arg0];
}
