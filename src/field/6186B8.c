#include "common.h"
#include "field/n22.h"

FldLightRec *FldPointLightGet(u8 idx) {
    if (idx < 3) {
        return NULL;
    }
    return &fldPointLights[idx - 3].light;
}
