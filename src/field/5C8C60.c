#include "common.h"

s32 TxtMsgCharToGlyph(u16 c) {
    s32 r;
    if (c >= 0x100) {
        r = (((c & 0xF00) >> 8) + 1) * 0xD0 - 0x20;
        return (c & 0xFF) + r;
    }
    return c - 0x20;
}
