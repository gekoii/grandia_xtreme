#include "common.h"
#include "field/n26.h"

s32 McLoadGetState(void) {
    return mcLoadState;
}

void McLoadSetState(s32 arg0) {
    mcLoadState = arg0;
}
