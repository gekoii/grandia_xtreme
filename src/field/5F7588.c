#include "common.h"
#include "field/state.h"

FldCamConfig *FldCamConfigGet(void) {
    return &fldCamConfig;
}

void FldCamConfigSetAux(s32 arg0) {
    fldCamConfigAux = arg0;
}
