#include "common.h"
#include "m2c_macros.h"

s32 GphProjTexCalcWorkSize(s32 arg0) {
    return (arg0 * 0x18) + 0x10;
}
