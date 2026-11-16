#include "common.h"
#include "field/state.h"

s32 FldTreasureGetItem(s32 idx) {
    FldTreasure *e = &fldTreasureWork[idx];

    if (e->state == 0) {
        return -1;
    }
    return e->item;
}
