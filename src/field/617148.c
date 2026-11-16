#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void FldSndBgmStop(void) {
    if (fldSndBgmHandle != 0xFFFFFFFF) {
        SndBgmFadeOut(fldSndBgmHandle, 0);
    }
}
