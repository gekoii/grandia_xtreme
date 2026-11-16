/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"


s64 strtol(s32 arg0, s32 arg1, s32 arg2) {
    return _strtol_r(_impure_ptr, arg0, arg1, arg2);
}
