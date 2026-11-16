/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"


s32 sceSifSetSreg(s32 arg0, s32 arg1) {
    _sceSifSregs[arg0] = arg1;
    return arg1;
}
