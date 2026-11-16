#include "common.h"
#include "field/state.h"


void FldSpotOpen(s32 i) {
    if (((FldSpot *)(i * sizeof(FldSpot) + (u32)fldSpotWork->list))->unkB == 0) {
        ((FldSpot *)(i * sizeof(FldSpot) + (u32)fldSpotWork->list))->unkB = 1;
        ((FldSpot *)(i * sizeof(FldSpot) + (u32)fldSpotWork->list))->unkE = 50;
    }
}
