#include "common.h"
#include "field/n13.h"

GphModel *FldObjFindById(s32 id) {
    u32 i;

    for (i = 0; i < fldModelTable.count; i++) {
        if ((s16)fldModelTable.items[i]->objId == id) {
            return fldModelTable.items[i];
        }
    }
    return NULL;
}
