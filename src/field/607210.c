#include "common.h"
#include "field/n21.h"

void AmapCellSetCorridor(s32 x, s32 y) {
    s32 i;

    i = y * fldMapWidth + x;
    fldMapCells[i] = 1;
    fldMapCells[fldMapLayerSize + i] = 2;
    fldMapCells[fldMapLayerSize * 2 + i] = 2;
    fldMapCells[fldMapLayerSize * 3 + i] = 0xFF;
}
