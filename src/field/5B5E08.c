#include "common.h"
#include "field/event.h"

OccsEntry *OccsFindById(s32 id) {
    s32 n;
    s32 i;
    OccsEntry *e;

    n = occsTable->count;
    for (i = 0; i < n; i++) {
        e = &occsTable->items[i];
        if (e->id == id) return e;
    }
    return NULL;
}
