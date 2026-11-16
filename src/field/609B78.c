#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

s32 AmapCellNotNearReserved(s32 x, s32 y, void *map) {
    u8 *p;

    if (MAP(2, x, y - 1) == 1) return 0;
    p = (u8 *)((x + (y * fldMapWidth + fldMapLayerSize * 2)) + (u32)fldMapCells);
    if (p[1] == 1) return 0;
    if (MAP(2, x, y + 1) == 1) return 0;
    if (p[-1] == 1) return 0;
    return 1;
}
