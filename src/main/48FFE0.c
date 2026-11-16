/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "m2c_macros.h"

M2C_UNK func_0048281C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */

s32 sceFsReset(void) {
    _sceFsInitDone = 0;
    func_0048281C((M2C_UNK *)_sceFsIopVersion, 0, 4);
    return 0;
}
