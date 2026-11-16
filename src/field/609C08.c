#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapTraceStepOpen(s32 x, s32 y, s32 *ox, s32 *oy, s32 *best) {
    s32 v;

    if (MAP(2, x, y) == 1) return;
    v = MAP(3, x, y);
    if (v < *best) {
        *best = v;
        *ox = x;
        *oy = y;
    }
}
