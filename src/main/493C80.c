/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

s32 sceSifGetReg(s32);

s32 sceSifSyncIop(void) {
    if (sceSifGetReg(4) & 0x40000) {
        _sceTtyReset();
        return 1;
    }
    return 0;
}
