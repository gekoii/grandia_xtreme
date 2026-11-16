#include "common.h"
#include "field/n17.h"

extern void GphModelUpdateGsRegs(struct GphModel *);

void FldSkyCalcPass(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (fldSkyTable[i].enable != 0 && fldSkyTable[i].model != NULL) {
            GphModelUpdateGsRegs(fldSkyTable[i].model);
        }
    }
}
