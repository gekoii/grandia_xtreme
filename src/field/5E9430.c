#include "common.h"
#include "field/n17.h"

s32 FldSkyIsEnabled(u8 arg0) {
    return fldSkyTable[arg0].enable;
}
