#include "common.h"
#include "party.h"

void PtySkillEquip(s32 arg0, s32 arg1, s32 arg2) {
    PtySkill *s = &ptyPartyData->skills[arg2];
    s32 idx = arg0 - 0x1001;
    if (s->id != 0) {
        PtyChar *u;
        s->owner = idx;
        u = &ptyCharData[idx];
        u->skills[arg1] = arg2;
    }
}
