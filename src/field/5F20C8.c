#include "common.h"
#include "party.h"

s16 PtyPartyFindChar(s16 id) {
    s32 i;
    PtyUnit *e;
    if (id == -1) {
        return -1;
    }
    e = ptyPartyUnits;
    for (i = 0; i < 4; i++, e++) {
        if (e->id - 0x1001 == id) {
            return i;
        }
    }
    return -1;
}
