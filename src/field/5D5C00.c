/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"

s32 FldTreasureFindNearest(void *pos) {
    s32 i;
    s32 best = -1;
    f32 min = 500.0f;
    f32 d;
    FldTreasure *e;
    FldTreasure *e2;

    for (i = 0; i < 64; i++) {
        e = &fldTreasureWork[i];
        if ((u32)(e->state - 2) < 2) {
            d = FldVecDist(pos, e->path[3]);
            e2 = &fldTreasureWork[i];
            e2->dist = d;
            if (d < 130.0f && d < min) {
                min = d;
                best = i;
            }
        }
    }
    return best;
}
