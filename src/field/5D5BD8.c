#include "common.h"
#include "field/state.h"

s32 FldTreasureIsIdle(u8 idx) {
    FldTreasure *e = &fldTreasureWork[idx];

    return e->state == 2;
}
