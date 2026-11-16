#include "common.h"
#include "field/n21.h"

void AmapCornerBitTest(s32 x, s32 y, s32 bit, s32 *m0, s32 *m1, AmapWork *a) {
    u8 v;

    if (x >= 0 && y >= 0 && x <= a->width - 1 && y <= a->height - 1) {
        v = fldMapCells[y * fldMapWidth + x];
        if (v == 0) *m0 |= bit;
        if (v == 2) *m1 |= bit;
    }
}
