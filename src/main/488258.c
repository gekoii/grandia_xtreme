/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/newlib.h"
#include "protos/main.h"


void _cleanup(void) {
    _cleanup_r((s32)_impure_ptr);
}
