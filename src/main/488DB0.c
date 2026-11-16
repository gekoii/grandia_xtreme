/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"


s32 setlocale(s32 arg0, s32 arg1) {
    return _setlocale_r(_impure_ptr, arg0, arg1);
}
