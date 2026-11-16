#include "common.h"
#include "field/n13.h"

MapEntrance *MapEntranceGet(s32 arg0) {
    if (arg0 < 0) {
        return NULL;
    }
    if (arg0 >= mapEntranceTable->count) {
        return NULL;
    }
    return &mapEntranceTable->items[arg0];
}
