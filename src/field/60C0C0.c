#include "common.h"
#include "protos/field.h"


void AmapCornerMask(s32 x, s32 y, s32 *o0, s32 *o1, void *a) {
    s32 m0;
    s32 m1;

    m1 = 0;
    m0 = 0;
    AmapCornerBitTest(x, y, 0x1, &m0, &m1, a);
    AmapCornerBitTest(x + 1, y, 0x2, &m0, &m1, a);
    AmapCornerBitTest(x + 1, y + 1, 0x4, &m0, &m1, a);
    AmapCornerBitTest(x, y + 1, 0x8, &m0, &m1, a);
    *o0 = m0;
    *o1 = m1;
}
