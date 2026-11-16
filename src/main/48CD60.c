/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s32 kCopy(s32 *arg0, s32 *arg1, u32 arg2) {
    u32 i;

    arg2 >>= 2;
    for (i = 0; i < arg2; i++) {
        *arg0++ = *arg1++;
    }
    return 0;
}
