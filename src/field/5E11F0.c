#include "common.h"
#include "protos/field.h"
#include "gph.h"


void PtclFldModelClearPartFlag2(void *arg0) {
    s32 i;
    GphPart *e;

    for (i = 0; i < 4; i++) {
        e = ChrModelFindPart(arg0, i + 1);
        if (e != NULL && (e->flags & 2)) {
            e->flags ^= 2;
        }
    }
}
