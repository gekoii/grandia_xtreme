#include "common.h"
#include "field/n22.h"

extern void GphModelUpdateGsRegs(void *);

void AmapModelCalcPass(void) {
    u32 i;

    if (amapState->loaded != 0) {
        i = 0;
        if (amapState->objModelCount != 0) {
            do {
                if (amapObjModels[i] != NULL) {
                    GphModelUpdateGsRegs(amapObjModels[i]);
                }
                i++;
            } while (i < amapState->objModelCount);
        }
    }
}
