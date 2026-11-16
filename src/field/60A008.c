#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]
#define MAPD(l, x, y, dx) fldMapCells[(dx) + fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapCorridorMarkAround(AmapWork *a) {
    s32 y;
    s32 x;

    for (y = 1; y < a->height - 1; y++) {
        for (x = 1; x < a->width - 1; x++) {
            if (MAP(1, x, y) >= 2) {
                MAP(2, x, y - 1) = 2;
                MAPD(2, x, y - 1, 1) = 2;
                MAPD(2, x, y, 1) = 2;
                MAPD(2, x, y + 1, 1) = 2;
                MAP(2, x, y + 1) = 2;
                MAPD(2, x, y + 1, -1) = 2;
                MAPD(2, x, y, -1) = 2;
                MAPD(2, x, y - 1, -1) = 2;
            }
        }
    }
}
