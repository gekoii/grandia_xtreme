/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "protos/main.h"


s32 sceMcMkdir(s32 a0, s32 a1, s32 a2) {
    s32 temp_v0;

    temp_v0 = sceMcOpen(a0, a1, a2, 0x40);
    if (temp_v0 == 0) {
        _sceMcCmdNo = 0xB;
    }
    return temp_v0;
}
