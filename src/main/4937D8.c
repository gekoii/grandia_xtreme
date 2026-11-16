/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s64 sceSifLoadStartModule(s32 a0, s32 a1, s32 a2, void *a3) {
    return _sceSifLoadModule(a0, a1, a2, a3, 0);
}
