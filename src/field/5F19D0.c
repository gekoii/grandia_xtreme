#include "common.h"
#include "party.h"

s16 ItemSkillAddLv1(s16 arg0) {
    s32 i;

    for (i = 0; i < 100; i++) {
        if (ptyPartyData->skills[i].id == 0) {
            ptyPartyData->skills[i].id = arg0;
            ptyPartyData->skills[i].owner = -1;
            ptyPartyData->skills[i].level = 1;
            ptyPartyData->skills[i].exp = 0;
            return i;
        }
    }
    return -1;
}
