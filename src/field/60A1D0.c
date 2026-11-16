#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapCorridorBlockAround(AmapWork *a) {
    s32 y;
    s32 x;

    for (y = 1; y < a->height - 1; y++) {
        for (x = 1; x < a->width - 1; x++) {
            if (MAP(1, x, y) >= 2) {
                AmapCellBlock(x, y - 1);
                AmapCellBlock(x, y - 1);
                AmapCellBlock(x + 1, y - 1);
                AmapCellBlock(x + 1, y);
                AmapCellBlock(x + 1, y + 1);
                AmapCellBlock(x, y + 1);
                AmapCellBlock(x - 1, y + 1);
                AmapCellBlock(x - 1, y);
                AmapCellBlock(x - 1, y - 1);
            }
        }
    }
}
