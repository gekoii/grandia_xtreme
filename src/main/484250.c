/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"


s32 strtod(s32 arg0, s32 arg1) {
    return ((s32 (*)(struct _reent *, s32, s32))_strtod_r)(_impure_ptr, arg0, arg1);
}
