#include "common.h"
#include "protos/field.h"
#include "field/state.h"

s32 FldTreasureDropSlotAlloc(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 best;
    u32 min;

    for (i = 0; i < 8; i++) {
        k = i + 32;
        if (fldTreasureWork[k].state == 0) {
            return k;
        }
    }
    min = 0xFFFF;
    best = -1;
    for (i = 0; i < 8; i++) {
        j = i + 32;
        if (fldTreasureWork[j].state) {
            if (fldTreasureWork[j].age < min) {
                min = fldTreasureWork[j].age;
                best = j;
            }
        }
    }
    if (best == -1) {
        return 32;
    }
    FldTreasureRemove(best);
    return best;
}
