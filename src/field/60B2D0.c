#include "common.h"
#include "m2c_macros.h"

s32 AmapMaskToFacing(s32 arg0) {
    s32 var_v1;

    var_v1 = 0x40;
    if (!(arg0 & 4)) {
        var_v1 = 0x80;
        if (!(arg0 & 0x10)) {
            var_v1 = !(arg0 & 0x40) ? 0 : 0xC0;
        }
    }
    return var_v1;
}
