#include "common.h"
#include "ptcl.h"
#include "field/n12.h"
#include "protos/field.h"

void GphModelUpdateGsRegs(GphModel *);

void PtclFldCalcPass(void) {
    s32 i;
    s32 j;

    if (ptclFldWork != NULL && ptclFldWork->system != NULL) {
        GphDrawListSet(fldPtclDrawList);
        PtclSystemSelect((s32)ptclFldWork->system);
        PtclModelsUpdateGsRegs();
        for (i = 0; i < 16; i++) {
            if (ptclFldWork->active[i]) {
                for (j = 0; j < 8; j++) {
                    if (ptclFldWork->models[i][j]) {
                        GphModelUpdateGsRegs(ptclFldWork->models[i][j]);
                    }
                }
            }
        }
    }
}
