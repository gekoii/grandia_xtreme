/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void FldTreasureRefreshNames(void) {
    s32 buf[8];
    s32 i;
    FldTreasure *e;
    FldTreasure *e2;
    FldTreasure *e3;
    u8 v;

    for (i = 0; i < 64; i++) {
        e = &fldTreasureWork[i];
        if (e->state >= 2 && !(e->t < 0.1f) && e->nameLen) {
            ItemInfoFill(buf, e->item, 0);
            e2 = &fldTreasureWork[i];
            v = TxtGlyphQueueStr(2, buf[3], e2->nameRow);
            e3 = &fldTreasureWork[i];
            e3->nameLen = v;
        }
    }
}
