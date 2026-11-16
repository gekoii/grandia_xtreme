#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapVoidGrow(AmapWork *a) {
    s32 x;
    s32 y;

    if (a->voidGrowRate == 0) return;
    if (VuRandInt(100) + 1 > a->voidGrowRate) return;
    for (y = 2; y < a->height - 2; y++) {
        for (x = 2; x < a->width - 2; x++) {
            if (fldMapCells[y * fldMapWidth + x] == 1) {
                AmapVoidGrowCell(x, y - 1, a);
                AmapVoidGrowCell(x + 1, y - 1, a);
                AmapVoidGrowCell(x + 1, y, a);
                AmapVoidGrowCell(x + 1, y + 1, a);
                AmapVoidGrowCell(x, y + 1, a);
                AmapVoidGrowCell(x - 1, y + 1, a);
                AmapVoidGrowCell(x - 1, y, a);
                AmapVoidGrowCell(x - 1, y - 1, a);
            }
        }
    }
}
