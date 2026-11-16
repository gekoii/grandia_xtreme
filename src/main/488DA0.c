/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK _lconv;

M2C_UNK *_localeconv_r(void) {
    return &_lconv;
}
