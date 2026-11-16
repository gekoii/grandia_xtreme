/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

u32 _sceMpegBsGetPtr(void *arg0, s32 arg1) {
    u32 var_v1;

    var_v1 = M2C_FIELD(arg0, s32 *, 8) + (arg1 >> 3);
    if (var_v1 >= (u32) M2C_FIELD(arg0, u32 *, 0x24)) {
        var_v1 -= M2C_FIELD(arg0, s32 *, 0x28);
    }
    return var_v1;
}
