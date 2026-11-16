#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void FldTreasureSetStateById(u8 id, u8 val) {
    s32 i;
    FldTreasure *e;
    FldTreasure *e2;

    for (i = 0; i < 64; i++) {
        e = &fldTreasureWork[i];
        if (e->active && e->id == id) {
            e->state = val;
            e2 = &fldTreasureWork[i];
            ChrInstSetVisible(e2->instGroup, e2->inst, val);
            return;
        }
    }
}
