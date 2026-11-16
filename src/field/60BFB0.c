#include "common.h"
#include "protos/field.h"


void AmapFloorMask8(s32 x, s32 y, s32 *out) {
    s32 m;

    m = 0;
    AmapFloorBitTest(x, y - 1, 0x1, &m);
    AmapFloorBitTest(x + 1, y - 1, 0x2, &m);
    AmapFloorBitTest(x + 1, y, 0x4, &m);
    AmapFloorBitTest(x + 1, y + 1, 0x8, &m);
    AmapFloorBitTest(x, y + 1, 0x10, &m);
    AmapFloorBitTest(x - 1, y + 1, 0x20, &m);
    AmapFloorBitTest(x - 1, y, 0x40, &m);
    AmapFloorBitTest(x - 1, y - 1, 0x80, &m);
    *out = m;
}
