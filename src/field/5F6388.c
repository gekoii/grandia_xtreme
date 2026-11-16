#include "common.h"
#include "party.h"

s32 ItemCharHasEquipAllSkill(s16 arg0) {
    s32 i;
    s32 j;
    s32 found;
    s16 id;
    s16 idx;
    u8 k;

    found = 0;
    for (i = 0; i < 15; i++) {
        id = ptyCharData[arg0].skills[i];
        if (id != -1) {
            idx = id;
            k = ptyPartyData->skills[id].id;
            for (j = 0; j < 3; j++) {
                if (ptySkillTable[k].effects[j] == 0xA7 && ptyPartyData->skills[idx].level != 0) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
    return found;
}
