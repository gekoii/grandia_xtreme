#include "common.h"
#include "protos/field.h"


s32 AmapCorridorTrace(s32 x, s32 y) {
    s32 best;
    s32 by;
    s32 bx;
    s32 n;
    s32 old;

    best = 0xFF; bx = x; by = y; n = 0;
    AmapCellSetCorridor(x, y);
    for (;;) {
        old = best;
        AmapTraceStepFree(x, y - 1, &bx, &by, &best);
        AmapTraceStepFree(x + 1, y, &bx, &by, &best);
        AmapTraceStepFree(x, y + 1, &bx, &by, &best);
        AmapTraceStepFree(x - 1, y, &bx, &by, &best);
        if (old == best) break;
        x = bx; y = by; n++;
        AmapCellSetCorridor(x, y);
    }
    return n;
}
