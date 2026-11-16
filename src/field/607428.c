#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapCellMapInit(AmapWork *m) {
    s32 i;

    fldMapCells = m->cells;
    fldMapWidth = m->width;
    fldMapHeight = m->height;
    AmapBoundsSet(0, 0, fldMapWidth, fldMapHeight);
    fldMapLayerSize = fldMapWidth * fldMapHeight;
    for (i = 0; i < fldMapLayerSize * 7; i++) {
        fldMapCells[i] = 0;
    }
}
