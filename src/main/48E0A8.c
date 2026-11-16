/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"


s32 sceSifGetSreg(s32 arg0) {
    return _sceSifSregs[arg0];
}
