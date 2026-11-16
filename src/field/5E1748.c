#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n12.h"


void PtclFldDraw(void) {
    if (ptclFldWork != NULL && ptclFldWork->system != NULL) {
        GphDrawListSet(fldPtclDrawList);
        PtclSystemSelect((s32)ptclFldWork->system);
        PtclDrawSprites(0);
    }
}
