#include "common.h"
#include "main/n04.h"

void VuMtxPoolReset(void) {
    vuMtxPool.cur = vuMtxPool.base;
}
