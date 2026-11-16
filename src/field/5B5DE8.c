#include "common.h"
#include "field/event.h"

OccsEntry *OccsGet(s32 arg0) {
    return &occsTable->items[arg0];
}
