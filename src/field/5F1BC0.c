#include "common.h"
#include "party.h"
#include "field/n19.h"
#include "protos/field.h"

s32 ItemSkillHasRoom(void) {
    s32 i;
    s32 n = 0;

    for (i = 0; i < 100; i++) {
        if (ptyPartyData->skills[i].id != 0) {
            if (ptyPartyData->skills[i].owner == -1 || PtyCharIsJoined(ptyPartyData->skills[i].owner) >= 0) {
                n++;
            }
        }
    }
    if (n < itemSkillCapacity) {
        return 0;
    }
    return -2;
}
