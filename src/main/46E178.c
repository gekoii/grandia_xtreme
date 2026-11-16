/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

extern s32 _sceDmaDebugMode;

s32 sceDmaDebug(s32 arg0) {
    s32 temp_v0;

    temp_v0 = _sceDmaDebugMode;
    _sceDmaDebugMode = arg0;
    return temp_v0;
}
