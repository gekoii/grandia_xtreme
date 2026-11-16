#include "common.h"
#include "gph.h"
#include "field/n17.h"

GphModel *MapModelFindById(s32 arg0) {
    s32 i;
    GphModel *p;

    for (i = 0; i < mapModelCount; i++) {
        p = mapModelTable[i].model;
        if (p->objId == arg0) {
            return p;
        }
    }
    return 0;
}
