/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "m2c_macros.h"

M2C_UNK func_0048281C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */

s32 sceSifLoadFileReset(void) {
    _sceSifLoadBindState = -1;
    func_0048281C((M2C_UNK *)_sceSifLoadIopVersion, 0, 4);
    return 0;
}
