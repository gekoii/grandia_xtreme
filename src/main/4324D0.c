#include "common.h"
#include "main/n04.h"

void VuMtxStackPop(void) {
    FMATRIX *top;

    top = vuMtxStack->cur - 1;
    if (top >= vuMtxStack->base) {
        vuMtxStack->cur = top;
    }
}

void VuMtxStackPopN(s32 arg0) {
    vuMtxStack->cur -= arg0;
}
