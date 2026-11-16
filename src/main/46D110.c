/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK _sceGsGParam;

M2C_UNK *sceGsGetGParam(void) {
    return &_sceGsGParam;
}
