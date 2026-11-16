#include "common.h"
#include "main/n09.h"
#include "m2c_macros.h"


s32 GsGetVblanksSinceFlip(void) {
    return gsVblanksSinceFlip;
}

s32 GsGetVblankCount(void) {
    return gsVblankCount;
}
