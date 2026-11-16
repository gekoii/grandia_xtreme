#include "common.h"
#include "field/n17.h"

struct GphModel *FldSkyGetModel(s32 arg0) {
    if (arg0 >= 3) {
        return NULL;
    }
    return fldSkyTable[arg0].model;
}
