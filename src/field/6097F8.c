#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]
#define MAPR(l, x, y) fldMapCells[(x) + (fldMapLayerSize * (l) + (y) * fldMapWidth) + 1]

s32 AmapCorridorTraceWide(s32 x, s32 y, AmapWork *m) {
    s32 best;
    s32 by;
    s32 bx;
    s32 n;
    s32 old;
    s32 r;

    best = 0xFF;
    bx = x;
    by = y;
    n = 0;
    AmapCellSetCorridor(x, y);
    for (;;) {
        r = AmapCorridorWideRoll(m);
        old = best;
        AmapTraceStepOpen(x, y - 1, &bx, &by, &best);
        AmapTraceStepOpen(x + 1, y, &bx, &by, &best);
        AmapTraceStepOpen(x, y + 1, &bx, &by, &best);
        AmapTraceStepOpen(x - 1, y, &bx, &by, &best);
        if (old == best) break;
        x = bx;
        y = by;
        n++;
        AmapCellSetCorridor(x, y);
        if (r) {
            if (MAPR(2, x, y) == 0 && x + 1 < m->width - 1 && AmapCellNotNearReserved(x + 1, y, m)) {
                fldMapCells[x + 1 + y * fldMapWidth] = 1;
                MAPR(1, x, y) = 2;
                MAPR(2, x, y) = 2;
            }
            if (MAP(2, x, y + 1) == 0 && AmapCellNotNearReserved(x, y + 1, m)) {
                MAP(0, x, y + 1) = 1;
                MAP(1, x, y + 1) = 2;
                MAP(2, x, y + 1) = 2;
            }
            if (MAPR(2, x, y + 1) == 0 && x + 1 < m->width - 1 && AmapCellNotNearReserved(x + 1, y + 1, m)) {
                fldMapCells[x + 1 + (y + 1) * fldMapWidth] = 1;
                MAPR(1, x, y + 1) = 2;
                MAPR(2, x, y + 1) = 2;
            }
        }
    }
    return n;
}
