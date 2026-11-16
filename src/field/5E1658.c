#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n12.h"


void PtclFldBuildDrawList(void) {
    s32 i;
    s32 j;

    if (ptclFldWork != NULL && ptclFldWork->system != NULL) {
        GphDrawListSet(fldPtclDrawList);
        GphDrawListClear(NULL);
        PtclSystemSelect((s32)ptclFldWork->system);
        PtclDrawModels(0);
        for (i = 0; i < 16; i++) {
            if (ptclFldWork->active[i]) {
                for (j = 0; j < 8; j++) {
                    if (ptclFldWork->models[i][j]) {
                        GphModelCalcAndDraw(ptclFldWork->models[i][j]);
                    }
                }
            }
        }
        GphDrawListSort();
    }
}
