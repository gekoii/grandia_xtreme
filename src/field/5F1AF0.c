#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemSkillDiscard(u16 idx) {
    s32 i;
    s32 o;
    if (ptyPartyData->skills[idx].id == 0) {
        return -1;
    }
    o = ptyPartyData->skills[idx].owner;
    if (o != -1) {
        for (i = 0; i < 15; i++) {
            if (ptyCharData[o].skills[i] == idx) {
                ptyCharData[o].skills[i] = -1;
                break;
            }
        }
        ItemCharDropUnusableEquip(o);
    }
    ptyPartyData->skills[idx].id = 0;
    ptyPartyData->skills[idx].owner = -1;
    ItemSkillListBuild(0, -1);
    return 0;
}
