#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapVoidFillAll(AmapWork *a) {
    s32 x;
    s32 y;

    for (y = 0; y < a->height; y++) {
        for (x = 0; x < a->width; x++) {
            if (MAP(1, x, y) != 1 && MAP(0, x, y) == 0) {
                MAP(0, x, y) = 2;
            }
        }
    }
    a->voidFilled = 1;
}
