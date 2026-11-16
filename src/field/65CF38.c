#include "common.h"
#include "field/n26.h"

s32 McSaveGetState(void) {
    return mcSaveState;
}

void McSaveChangeState(s32 arg0) {
    s32 temp_v1;

    temp_v1 = mcSaveState;
    mcSaveState = arg0;
    mcSavePrevState = temp_v1;
}

void McSaveSetState(s32 arg0) {
    mcSaveState = arg0;
}
