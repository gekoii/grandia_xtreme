#include "common.h"
#include "field/n17.h"

void FldLensFlareInit(void) {
    fldLensFlareEnable = 0;
    fldLensFlareAlpha = 1.0f;
    fldSunPos[0] = 10000.0f;
    fldSunPos[1] = 6000.0f;
    fldSunPos[2] = 10000.0f;
}
