#include "common.h"
#include "field/event.h"

s16 OccGetState(s16 i) {
    if (i < occTable->count) {
        return occTable->items[i].state;
    }
    return -1;
}
