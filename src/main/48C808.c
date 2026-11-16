/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

s32 fstat(s32 arg0, u8 *arg1) {
    *(s64 *)(arg1 + 0x48) = 0;
    *(s32 *)(arg1 + 4) = 0x2000;
    return 0;
}
