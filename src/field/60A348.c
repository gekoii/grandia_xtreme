#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapCellBlock(s32 x, s32 y) {
    if (MAP(2, x, y) == 0) {
        MAP(2, x, y) = 3;
        MAP(3, x, y) = 0xFF;
    }
}
