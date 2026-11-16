#include "common.h"
#include "field/n21.h"

void AmapFloorBitTest(s32 x, s32 y, s32 bit, s32 *m) {
    if (fldMapCells[y * fldMapWidth + x] == 1) {
        *m |= bit;
    }
}
