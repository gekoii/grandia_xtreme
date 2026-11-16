#include "common.h"
#include "field/state.h"
#include "field/n12.h"
#include "protos/field.h"

extern void GphModelUpdateGsRegs(struct GphModel *);

void ChrInstCalcPass(void) {
    s32 i, j;

    GphDrawListSet(fldInstDrawList);
    if (chrInstPool->count != 0) {
        for (i = 0; i < 48; i++) {
            for (j = 0; j < 32; j++) {
                if (chrInstPool->slots[i][j].used != 0 && chrInstPool->slots[i][j].visible != 0) {
                    GphModelUpdateGsRegs(chrInstPool->slots[i][j].model);
                }
            }
        }
    }
}
