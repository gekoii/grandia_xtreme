#include "common.h"
#include "field/n17.h"

s32 MapModelGetCount(void) {
    return mapModelCount;
}

GphModel *MapModelGetObj(s32 arg0) {
    if (mapModelCount < arg0) {
        return NULL;
    }
    return mapModelTable[arg0].model;
}
