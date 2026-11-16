/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s32 sceSifLoadElfPart(s32 arg0, s32 arg1, s32 arg2) {
    return _sceSifLoadElfPart(arg0, arg1, arg2, 1);
}
