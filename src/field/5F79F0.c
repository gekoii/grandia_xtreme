#include "common.h"
#include "field/n19.h"
#include "protos/field.h"

void FldLightPresetLoad(s32 arg0) {
    fldLightPresetTable = (FldLightPreset *)arg0;
    FldLightPresetChange(0, 0.0f);
}
