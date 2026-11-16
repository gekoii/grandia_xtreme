#include "common.h"
#include "main/n04.h"

void VuMtxPoolInit(s32 arg0, s32 arg1) {
    vuMtxPool.base = (FMATRIX *)arg1;
    vuMtxPool.cur = (FMATRIX *)arg1;
    vuMtxPool.count = arg0;
}
