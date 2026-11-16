#include "common.h"
#include "protos/field.h"


void MapMiniAutoExploredClear(void) {
    s32 i;

    for (i = 0; i < 0x100; i++) {
        MapMiniExploredSet(0x2F, i, 0);
    }
}
