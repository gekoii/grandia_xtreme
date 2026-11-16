#include "common.h"
#include "party.h"

s32 PtySkillAdd(s32 arg0) {
    s32 i;
    PtySkill *p = ptyPartyData->skills;

    for (i = 0; i < 100; i++, p++) {
        if (p->id == 0) {
            break;
        }
    }
    if (i >= 100) {
        return -1;
    }
    p->id = arg0;
    p->owner = -1;
    p->level = 0;
    p->exp = 0;
    return i;
}
