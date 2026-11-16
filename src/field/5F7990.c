#include "common.h"
#include "field/n19.h"
#include "protos/field.h"

extern s16 fldLightPresetCur;

void FldLightPresetChange(u8 arg0, f32 f) {
    if (f < 1.0f) {
        FldLightPresetApply(arg0);
        fldLightPresetCur = arg0;
        return;
    }
    fldLightFade.active = 1;
    fldLightFade.to = arg0;
    fldLightFade.frames = f;
    fldLightFade.t = 0;
}
