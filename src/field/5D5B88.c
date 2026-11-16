#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void FldTreasureOpen(u8 idx) {
    FldTreasure *e = &fldTreasureWork[idx];

    if (e->state == 3) {
        e->state = 4;
        SndSePlay(0x7F8, 0, 0, 10, 0);
    }
}
