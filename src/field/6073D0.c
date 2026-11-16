#include "common.h"
#include "field/n21.h"

void AmapCellReserve(s32 x, s32 y) {
    s32 i;

    i = y * fldMapWidth + x;
    fldMapCells[fldMapLayerSize * 2 + i] = 1;
    fldMapCells[fldMapLayerSize * 3 + i] = 0xFF;
}
