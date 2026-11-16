#include "common.h"
#include "party.h"

s32 PtySkillBookAdd(s32 arg0) {
    s32 i;
    PtySkillBook *p = ptyPartyData->books;

    for (i = 0; i < 24; i++, p++) {
        if (p->itemId <= 0) {
            break;
        }
    }
    if (i >= 24) {
        return -1;
    }
    p->itemId = arg0;
    p->owner = -1;
    return i;
}
