#include "common.h"
#include "battle/unit.h"

BtlUnit *BtlEnemyFindUnit(s32 id, s32 start) {
    BtlUnit *u;
    s32 i;

    if (start <= 3) {
        start = 4;
    }
    u = &btlUnits[start];
    for (i = start; i < 16; i++, u++) {
        if (u->state >= 4 && u->def->id == id) {
            return u;
        }
    }
    return NULL;
}
