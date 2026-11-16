/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

s32 func_00481AE0(s32);

s64 strtof(void) {
    return func_00481AE0(((s32 (*)(void))strtod)());
}
