#include "common.h"
#include "field/n21.h"

void AmapLayerClear(s32 layer) {
    s32 i;
    u8 v;

    v = 0xFF;
    if (layer != 3) v = 0;
    for (i = 0; i < fldMapLayerSize; i++) {
        fldMapCells[layer * fldMapLayerSize + i] = v;
    }
}
