#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP1(l, x, y, dx) fldMapCells[(dx) + fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]
#define MAP0(l, x, y, dx) fldMapCells[(fldMapLayerSize * (l) + ((x) + (dx))) + (y) * fldMapWidth]

s32 AmapCorridorTrace8(s32 x, s32 y, AmapWork *a) {
    s32 best;
    s32 cy;
    s32 cx;
    s32 n;
    s32 flag;
    s32 prev;

    best = 0xFF;
    cx = x;
    cy = y;
    n = 0;
    AmapCellSetCorridor(x, y);
    for (;;) {
        flag = AmapCorridorWideRoll(a);
        prev = best;
        AmapTraceStepOpen(x, y - 1, &cx, &cy, &best);
        AmapTraceStepOpen(x + 1, y - 1, &cx, &cy, &best);
        AmapTraceStepOpen(x + 1, y, &cx, &cy, &best);
        AmapTraceStepOpen(x + 1, y + 1, &cx, &cy, &best);
        AmapTraceStepOpen(x, y + 1, &cx, &cy, &best);
        AmapTraceStepOpen(x - 1, y + 1, &cx, &cy, &best);
        AmapTraceStepOpen(x - 1, y, &cx, &cy, &best);
        AmapTraceStepOpen(x - 1, y - 1, &cx, &cy, &best);
        if (prev == best) break;
        x = cx;
        y = cy;
        n++;
        AmapCellSetCorridor(x, y);
        if (flag) {
            if (MAP1(2, x, y, 1) == 0 && x + 1 < a->width - 1) {
                MAP1(0, x, y, 1) = 1;
                MAP1(1, x, y, 1) = 2;
                MAP1(2, x, y, 1) = 2;
            }
            if (MAP0(2, x, y + 1, 0) == 0) {
                MAP0(0, x, y + 1, 0) = 1;
                MAP0(1, x, y + 1, 0) = 2;
                MAP0(2, x, y + 1, 0) = 2;
            }
            if (MAP1(2, x, y + 1, 1) == 0 && x + 1 < a->width - 1) {
                MAP1(0, x, y + 1, 1) = 1;
                MAP1(1, x, y + 1, 1) = 2;
                MAP1(2, x, y + 1, 1) = 2;
            }
        }
    }
    return n;
}
