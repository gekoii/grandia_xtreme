#include "common.h"
#include "protos/field.h"
#include "field/n17.h"


void FldSkyUpdate(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (fldSkyTable[i].enable != 0 && fldSkyTable[i].model != NULL) {
            ChrMotUpdate(fldSkyTable[i].model, 1.0f);
        }
    }
}
