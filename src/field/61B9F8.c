#include "common.h"
#include "field/state.h"

s32 FldSpotGetList(void) {
    return (s32)fldSpotWork->list;
}

void FldSpotDisable(s32 arg0) {
    fldSpotWork->list[arg0].unkA = 0;
}
