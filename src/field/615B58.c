#include "common.h"

s32 AmapObjRecGetType(u8 *arg0) {
    s32 r;

    if (arg0 == NULL) {
        r = -1;
    } else {
        r = *arg0 & 0x3F;
    }
    return r;
}
