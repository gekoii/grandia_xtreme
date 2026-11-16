#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapVoidGrowCell(s32 x, s32 y, void *a) {
    if (fldMapCells[x + y * fldMapWidth] == 0 && MAP(2, x, y) == 0 && AmapCellIsJoint(x, y, a) == 0) {
        fldMapCells[y * fldMapWidth + x] = 2;
        MAP(6, x, y) = 2;
    }
}
