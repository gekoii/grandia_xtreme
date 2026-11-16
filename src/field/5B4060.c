#include "common.h"
#include "field/n14.h"

void MapPathClearSlots(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        mapPathWork->smallUsed[i] = 0;
        mapPathWork->bigUsed[i] = 0;
    }
}
