#include "common.h"
#include "field/n13.h"

/* u16 id (an s32 here changes this function); both callers pass an s32 through a cast, unmasked. */
MapEntrance *MapEntranceFindById(u16 id) {
    u32 i;
    u32 n = mapEntranceTable->count;
    MapEntrance *p = mapEntranceTable->items;

    for (i = 0; i < n; p++) {
        i++;
        if (p->id == id) {
            return p;
        }
    }
    if (mapEntranceTable->count != 0) {
        return mapEntranceTable->items;
    }
    return NULL;
}
