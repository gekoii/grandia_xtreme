#include "common.h"
#include "field/n21.h"

void AmapTraceStepFree(s32 x, s32 y, s32 *bx, s32 *by, s32 *best) {
    s32 i;
    s32 v;

    i = y * fldMapWidth;
    if (fldMapCells[fldMapLayerSize * 2 + x + i] == 0) {
        v = fldMapCells[fldMapLayerSize * 3 + x + i];
        if (v < *best) {
            *best = v;
            *bx = x;
            *by = y;
        }
    }
}
