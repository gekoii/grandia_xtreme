/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s32 sceSifLoadModule(s32 a0, s32 a1, void *a2) {
    u8 buf[16];
    return _sceSifLoadModule(a0, a1, a2, buf, 0);
}
