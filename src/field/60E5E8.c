#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (y) * fldMapWidth + (x)]

void AmapStartExitReserve(AmapWork *a) {
    s32 y;
    s32 x;
    s32 cx;
    s32 cy;
    s32 px;

    cx = a->startX;
    cy = a->startY;
    for (y = -3; y < 4; y++) {
        for (x = -3; x < 4; x++) {
            px = cx + x;
            MAP(4, px, cy + y) = 1;
        }
    }
    cx = a->exitX;
    cy = a->exitY;
    for (y = -2; y < 3; y++) {
        for (x = -2; x < 3; x++) {
            px = cx + x;
            MAP(4, px, cy + y) = 1;
        }
    }
}
