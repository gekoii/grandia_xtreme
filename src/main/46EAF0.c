/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

s32 sceDmaRestart(u32 *arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = *arg0;
    *arg0 = arg1;
    return (temp_v0 >> 8) & 1;
}
