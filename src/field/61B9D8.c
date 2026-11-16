#include "common.h"
#include "field/state.h"

s32 FldSpotGetModel(s32 i) {
    return (s32)((FldSpot *)(i * sizeof(FldSpot) + (u32)fldSpotWork->list))->model;
}
