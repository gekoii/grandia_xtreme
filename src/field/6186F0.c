#include "common.h"
#include "field/n22.h"

void FldPointLightSetEnable(u8 idx, u8 mode) {
    if (idx >= 3) {
        if (mode == 2) {
            if (fldPointLights[idx - 3].enable == 1) {
                fldPointLights[idx - 3].enable = 0;
            } else {
                fldPointLights[idx - 3].enable = 1;
            }
        } else if (mode != 0) {
            fldPointLights[idx - 3].enable = 1;
        } else {
            fldPointLights[idx - 3].enable = 0;
        }
    }
}
