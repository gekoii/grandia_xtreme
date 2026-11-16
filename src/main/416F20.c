#include "common.h"

u16 TxtCharToGlyph(u16 arg0) {
    if (arg0 >= 0x100) {
        return (arg0 & 0xFF) + (u16)((((arg0 & 0xF00) >> 8) + 1) * 0xD0 - 0x20);
    }
    return arg0 - 0x20;
}
