#include "common.h"
#include "m2c_macros.h"
#include "field/n21.h"

void AmapBoundsSet(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 == -1) {
        amapBoundX0 = 0;
    } else {
        amapBoundX0 = arg0;
    }
    if (arg1 == -1) {
        amapBoundY0 = 0;
    } else {
        amapBoundY0 = arg1;
    }
    if (arg2 == -1) {
        amapBoundX1 = fldMapWidth;
    } else {
        amapBoundX1 = arg2;
    }
    if (arg3 == -1) {
        amapBoundY1 = fldMapHeight;
        return;
    }
    amapBoundY1 = arg3;
}
