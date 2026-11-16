#include "common.h"
#include "field/n21.h"

void AmapStartExitMark(AmapWork *a) {
    fldMapCells[a->startX + a->startY * fldMapWidth] = 2;
    fldMapCells[a->exitX + a->exitY * fldMapWidth] = 2;
}
