#include "common.h"
#include "field/n17.h"

void FldLensFlareSet(s32 x, s32 y, s32 z, s32 f) {
    fldSunPos[0] = x;
    fldLensFlareEnable = f;
    fldSunPos[1] = y;
    fldSunPos[2] = z;
}
