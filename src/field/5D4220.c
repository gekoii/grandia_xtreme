#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void *FldTreasureGetModelById(u8 id) {
    s32 i;
    FldTreasure *e;

    for (i = 0; i < 64; i++) {
        e = &fldTreasureWork[i];
        if (e->active && e->id == id) {
            return ChrInstGetPos(fldTreasureWork[i].instGroup, fldTreasureWork[i].inst);
        }
    }
    return NULL;
}
