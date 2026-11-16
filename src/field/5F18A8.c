#include "common.h"
#include "field/n19.h"
#include "party.h"
#include "protos/field.h"

s16 ItemSkillAdd(s16 arg0) {
    s32 i;
    s32 n = 0;

    for (i = 0; i < 100; i++) {
        if (ptyPartyData->skills[i].id != 0) {
            if (ptyPartyData->skills[i].owner == -1 || PtyCharIsJoined(ptyPartyData->skills[i].owner) >= 0) {
                n++;
            }
        }
    }
    if (n >= itemSkillCapacity) {
        return -2;
    }
    for (i = 0; i < 100; i++) {
        if (ptyPartyData->skills[i].id == 0) {
            ptyPartyData->skills[i].id = arg0;
            ptyPartyData->skills[i].owner = -1;
            ptyPartyData->skills[i].level = 0;
            ptyPartyData->skills[i].exp = 0;
            return i;
        }
    }
    return -1;
}
