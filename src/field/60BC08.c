#include "common.h"
#include "protos/field.h"


s32 AmapCellIsJoint(s32 x, s32 y, void *a) {
    s32 m0;
    s32 m1;

    m1 = 0;
    m0 = 0;
    AmapCornerBitTest(x, y - 1, 0x1, &m0, &m1, a);
    AmapCornerBitTest(x + 1, y - 1, 0x4, &m0, &m1, a);
    AmapCornerBitTest(x + 1, y, 0x10, &m0, &m1, a);
    AmapCornerBitTest(x + 1, y + 1, 0x40, &m0, &m1, a);
    AmapCornerBitTest(x, y + 1, 0x100, &m0, &m1, a);
    AmapCornerBitTest(x - 1, y + 1, 0x400, &m0, &m1, a);
    AmapCornerBitTest(x - 1, y, 0x1000, &m0, &m1, a);
    AmapCornerBitTest(x - 1, y - 1, 0x4000, &m0, &m1, a);
    m0 |= m1 << 1;
    if ((m0 & 0x3F) == 0x19) return 1;
    if ((m0 & 0x3F0) == 0x190) return 1;
    if ((m0 & 0x3F00) == 0x1900) return 1;
    if ((m0 & 0xF003) == 0x9001) return 1;
    if ((m0 & 0x3F) == 0x8) return 1;
    if ((m0 & 0x3F0) == 0x80) return 1;
    if ((m0 & 0x3F00) == 0x800) return 1;
    if ((m0 & 0xF003) == 0x8000) return 1;
    if ((m0 & 0x3F) == 0x9) return 1;
    if ((m0 & 0x3F0) == 0x90) return 1;
    if ((m0 & 0x3F00) == 0x900) return 1;
    if ((m0 & 0xF003) == 0x9000) return 1;
    if ((m0 & 0x3F) == 0x18) return 1;
    if ((m0 & 0x3F0) == 0x180) return 1;
    if ((m0 & 0x3F00) == 0x1800) return 1;
    if ((m0 & 0xF003) == 0x8001) return 1;
    if ((m0 & 0x3F) == 0x4) return 1;
    if ((m0 & 0x3F0) == 0x40) return 1;
    if ((m0 & 0x3F00) == 0x400) return 1;
    if ((m0 & 0xF003) == 0x4000) return 1;
    if ((m0 & 0x3F) == 0x11) return 1;
    if ((m0 & 0x3F0) == 0x110) return 1;
    if ((m0 & 0x3F00) == 0x1100) return 1;
    return (m0 & 0xF003) == 0x1001;
}
