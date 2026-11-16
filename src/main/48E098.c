/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

void _sceSifCmdChangeAddr(void *arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 8) = (s32) M2C_FIELD(arg0, s32 *, 0x10);
}
