#include "common.h"
#include "field/event.h"

OccEntry *OccGet(s32 arg0) {
    return &occTable->items[arg0];
}
