#include "common.h"
#include "field/n13.h"

GphModel *FldObjGet(s32 arg0) {
    if (arg0 < 0) {
        return NULL;
    }
    if ((u32)arg0 < fldModelTable.count) {
        return fldModelTable.items[arg0];
    }
    return NULL;
}
