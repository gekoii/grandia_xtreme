#include "common.h"
#include "main/n04.h"

void VuMtxStackInit(s32 arg0, FMATRIX *arg1) {
    vuMtxStack->cur = arg1;
    vuMtxStack->end = arg1 + arg0;
    vuMtxStack->count = arg0;
    vuMtxStack->base = arg1;
}
