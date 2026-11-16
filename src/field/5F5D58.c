#include "common.h"
#include "party.h"

s32 ItemCharSetSkill(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    if (ptyCharData[arg0].slots[arg1] == -1) {
        return -1;
    }
    ptyCharData[arg0].skills[(arg1 - ptyCharDefTable[arg0].eggSlots) * 3 + arg2] = arg3;
    ptyPartyData->skills[arg3].owner = arg0;
    return -1;
}
