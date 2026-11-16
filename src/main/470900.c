/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "m2c_macros.h"


void _sceMcGetInfoApdx(s32 arg0) {
    s32 temp_a0;

    temp_a0 = arg0 | 0x20000000;
    if (_sceMcGetInfoType != NULL) {
        *_sceMcGetInfoType = M2C_FIELD(temp_a0, s32 *, 0);
    }
    if (_sceMcGetInfoFree != NULL) {
        *_sceMcGetInfoFree = M2C_FIELD(temp_a0, s32 *, 4);
    }
    if (_sceMcGetInfoFormat != NULL) {
        *_sceMcGetInfoFormat = M2C_FIELD(temp_a0, s32 *, 0x90);
    }
}
