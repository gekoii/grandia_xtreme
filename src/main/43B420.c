#include "common.h"
#include "gph.h"

GphPart *ChrModelPartAt(GphModel *list, s32 index) {
    GphPart *entries = list->parts;

    if (index >= list->partCount) {
        return NULL;
    }
    return &entries[index];
}
