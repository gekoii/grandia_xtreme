/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

s32 hi0bits(u32 x) {
    s32 n = 0;
    if (!(x & 0xFFFF0000)) {
        n = 0x10;
        x <<= 16;
    }
    if (!(x & 0xFF000000)) {
        n += 8;
        x <<= 8;
    }
    if (!(x & 0xF0000000)) {
        n += 4;
        x <<= 4;
    }
    if (!(x & 0xC0000000)) {
        n += 2;
        x <<= 2;
    }
    if (!(x & 0x80000000)) {
        n++;
        if (!(x & 0x40000000)) {
            return 0x20;
        }
    }
    return n;
}
