/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void sceSifLoadModuleBuffer(s32 a0, s32 a1, s32 a2) {
    u8 buf[16];
    _sceSifLoadModuleBuffer(a0, a1, a2, buf);
}
