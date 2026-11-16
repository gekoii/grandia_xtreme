#include "common.h"
#include "field/n17.h"

void FldSkySetEnable(u8 arg0, u8 arg1) {
    fldSkyTable[arg0].enable = arg1;
}
