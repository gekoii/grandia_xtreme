#include "common.h"
#include "field/n21.h"

void AmapCellSetRoom(s32 x, s32 y, u8 v) {
    s32 i;

    i = y * fldMapWidth + x;
    fldMapCells[i] = v;
    fldMapCells[fldMapLayerSize + i] = 1;
}
