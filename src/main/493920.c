/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern u8 _sceSifLoadAllSecStr[];


s64 sceSifLoadElf(s32 arg0, s32 arg1) {
    return _sceSifLoadElfPart(arg0, _sceSifLoadAllSecStr, arg1, 1);
}
