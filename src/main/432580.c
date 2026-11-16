#include "common.h"
#include "main/n04.h"

s32 VuMtxPoolAlloc(void) {
    FMATRIX *old = vuMtxPool.cur;
    vuMtxPool.cur = old + 1;
    return (s32)old;
}
