#include "common.h"
#include "protos/field.h"
#include "ptcl.h"




void PtclFldModelSpinParts(void *arg0) {
    s32 i;
    GphPart *e;

    for (i = 0; i < 2; i++) {
        e = ChrModelFindPart(arg0, i + 1);
        if (e != NULL) {
            e->rot[1] = ptclFldWork->spinA;
        }
        e = ChrModelFindPart(arg0, i + 3);
        if (e != NULL) {
            e->rot[1] = ptclFldWork->spinB;
        }
    }
}
