#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapFloodFill(s32 x, s32 y, AmapWork *a);

void AmapFloodFillEmpty(s32 x0, s32 y0, AmapWork *a) {
    s32 y;
    s32 x;

    for (y = 0; y < a->height; y++) {
        for (x = 0; x < a->width; x++) {
            if (MAP(1, x, y)) {
                MAP(3, x, y) = 0xFF;
            } else {
                MAP(3, x, y) = 0;
            }
        }
    }
    AmapFloodFill(x0, y0, a);
}
