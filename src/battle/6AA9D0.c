#include "common.h"

u32 BtlChrMotionAltIndex(u32 arg0) {
    switch (arg0) {
    case 0:
        return 7U;
    case 3:
        return 8U;
    case 1:
    case 2:
        return 6U;
    case 4:
        return 0xBU;
    case 10:
        return 9U;
    default:
        return arg0;
    }
}
