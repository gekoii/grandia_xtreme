#include "common.h"
#include "main/n04.h"

s32 VuMtxPoolAllocN(s32 arg0) {
    FMATRIX *old = vuMtxPool.cur;
    vuMtxPool.cur = old + arg0;
    return (s32)old;
}
