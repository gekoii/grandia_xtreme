#include "common.h"
#include "field/event.h"

s32 OccIsActive(s16 i) {
    if (i < occTable->count) {
        if (occTable->items[i].flags & OCC_ACTIVE) {
            return 1;
        }
    }
    return 0;
}
