#include "common.h"
#include "field/n13.h"

void *FldMemAlloc(s32);

void MapEntranceAlloc(s32 arg0) {
    void *p;

    mapEntranceTable->count = arg0;
    if (arg0 != 0) {
        p = FldMemAlloc(arg0 << 6);
        if (p != NULL) {
            mapEntranceTable->items = p;
        } else {
            mapEntranceTable->count = 0;
            mapEntranceTable->items = NULL;
        }
        mapEntranceCur = 0;
    }
}
