/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"

M2C_UNK _sceMpegBsFlushBits(s32, s32);                    /* extern */

u32 _sceMpegBsGetBits(s32 arg0, s32 arg1) {
    s32 temp_s2;

    temp_s2 = ((s32 (*)(void))_sceMpegBsShowBits)();
    _sceMpegBsFlushBits(arg0, arg1);
    return temp_s2;
}
