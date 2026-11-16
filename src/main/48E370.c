/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "m2c_macros.h"

M2C_UNK RemoveDmacHandler(M2C_UNK, s32);                /* extern */
M2C_UNK DisableDmac(M2C_UNK);                     /* extern */

void sceSifExitCmd(void) {
    DisableDmac(5);
    RemoveDmacHandler(5, _sceSifCmdDmacHandlerId);
    _sceSifCmdInitDone = 0;
}
