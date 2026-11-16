#include "common.h"
#include "battle/unit.h"

BtlUnit *BtlPartyFindUnit(s32 id) {
    BtlUnit *u;
    s32 i;

    u = btlUnits;
    for (i = 0; i < 4; i++, u++) {
        if (u->state >= 4 && u->def->id == id) {
            return u;
        }
    }
    return NULL;
}
