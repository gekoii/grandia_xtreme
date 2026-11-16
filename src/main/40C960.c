#include "common.h"
#include "m2c_macros.h"
#include "gph.h"

void GphShatterStop(void) {
    gphShatter.active = 0;
}

s32 GphShatterIsActive(void) {
    return gphShatter.active;
}
