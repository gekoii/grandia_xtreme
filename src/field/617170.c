#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void FldSndStreamStop(void) {
    if (fldSndStreamHandle != 0xFFFFFFFF) {
        SndBgmFadeOut(fldSndStreamHandle, 0);
    }
}
