#include "common.h"
#include "battle/unit.h"

s32 BtlUnitAttachPart(s32 id, s32 v) {
    BtlUnit *u = btlUnits;
    s32 i;

    for (i = 0; i < 16; i++, u++) {
        if (u->state > 0 && u->def->id == id && u->body < 0) {
            u->body = v;
            return (s8)i;
        }
    }
    return -1;
}
