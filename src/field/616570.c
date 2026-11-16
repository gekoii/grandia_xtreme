#include "common.h"
#include "protos/field.h"
#include "field/n22.h"


void AmapUpdate(void) {
    u32 i;

    if (amapState->loaded != 0) {
        i = 0;
        AmapSwitchFlagsUpdate(&amapWork);
        if (amapState->objModelCount != 0) {
            do {
                if (amapObjModels[i] != NULL) {
                    ChrModelUpdate(amapObjModels[i], 1.0f);
                }
                i++;
            } while (i < amapState->objModelCount);
        }
    }
}
