#include "common.h"
#include "field/event.h"


OccEntry *OccFindById(s32 id) {
    s32 n;
    s32 i;
    OccEntry *e;

    n = occTable->count;
    for (i = 0; i < n; i++) {
        e = &occTable->items[i];
        if (e->id == id) return e;
    }
    return NULL;
}
