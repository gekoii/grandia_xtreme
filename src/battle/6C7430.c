#include "common.h"
#include "party.h"

s32 BtlMoveMemberCount(s16 arg0) {
    PtyMove *e;
    s32 i;

    e = &ptyMoveTable[arg0];
    if (arg0 < 0xDD) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        if (e->members[i] <= 0) {
            return (s8)i;
        }
    }
    return 4;
}
