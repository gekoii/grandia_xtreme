#include "common.h"
#include "main/n04.h"

void VuMtxStackSet(s32 arg0) {
    vuMtxStack = (VuMtxStack *)arg0;
}

s32 VuMtxStackSize(s32 arg0) {
    return arg0 << 6;
}
