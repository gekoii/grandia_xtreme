#include "common.h"
#include "field/n21.h"

void AmapSolidBitTest(s32 x, s32 y, s32 bit, s32 *m, AmapWork *a) {
    if (x >= 0 && y >= 0 && x < a->width && y < a->height && fldMapCells[y * fldMapWidth + x] != 0) {
        *m |= bit;
    }
}
