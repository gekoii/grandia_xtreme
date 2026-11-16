/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/newlib.h"
#include "m2c_macros.h"


void srand(s32 arg0) {
    _impure_ptr->_new._reent._rand_next = arg0;
}

s32 rand(void) {
    s32 temp_v1;

    temp_v1 = (_impure_ptr->_new._reent._rand_next * 0x41C64E6D) + 0x3039;
    _impure_ptr->_new._reent._rand_next = temp_v1;
    return temp_v1 & 0x7FFFFFFF;
}
