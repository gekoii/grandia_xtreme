#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapNodeRandomNear(s32 x, s32 y, s32 *ox, s32 *oy, s32 r, AmapWork *a) {
    s32 nx;
    s32 ny;

    r *= 2;
    do {
        do {
            nx = VuRandInt(r);
            ny = VuRandInt(r);
            nx = nx - r + x + 2;
            ny = ny - r + y + 2;
        } while (nx < 2 || ny < 2 || nx > a->width - 2 || ny > a->height - 2);
    } while (MAP(2, nx, ny) != 0);
    AmapCellSetNode(x, y);
    *ox = nx;
    *oy = ny;
}
