#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void FldSndEmitterEnableAll(u8 on) {
    u32 i;

    for (i = 0; i < fldSndEmitterCount; i++) {
        FldSndEmitterEnable(i, on);
    }
}
