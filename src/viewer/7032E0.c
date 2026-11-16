/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

s32 vwr_matherr(void *arg0) {
    s64 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s64 *, 8);
    dpcmp(temp_a0, temp_a0);
    return 0;
}
