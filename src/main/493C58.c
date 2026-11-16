/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
s32 sceSifGetReg(s32);
s32 sceSifIsAliveIop(void) {
    if (sceSifGetReg(4) & 0x10000) {
        return 1;
    }
    return 0;
}
