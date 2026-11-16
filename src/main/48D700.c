/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s32 _sceSioPutRawChar(s32 c) {
    while (*(volatile u32 *)0x1000F130 & 0x8000) {
    }
    *(volatile u8 *)0x1000F180 = c;
    return c;
}
