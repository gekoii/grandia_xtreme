#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]
#define MAPD(l, x, y, dx) fldMapCells[(dx) + fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

s32 AmapCorridorNeighbourCount(s32 x, s32 y, s32 *dir, AmapWork *a) {
    s32 n;

    n = 0;
    if (x >= a->width - 2 || x <= 0) return 0;
    if (y >= a->height - 2 || y <= 0) return 0;
    {
        if (MAP(1, x, y - 1) == 2) {
            *dir = 0;
            n++;
        }
        if (MAPD(1, x, y, 1) == 2) {
            n++;
            *dir = 1;
        }
        if (MAP(1, x, y + 1) == 2) {
            *dir = 2;
            n++;
        }
        if (MAPD(1, x, y, -1) == 2) {
            n++;
            *dir = 3;
        }
    }
    return n;
}
