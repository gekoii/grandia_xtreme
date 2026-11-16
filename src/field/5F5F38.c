/* cflags: -fno-rerun-loop-opt */
/* -fno-rerun-loop-opt: without it the second loop pass turns the 3-slot loop into a count-down (bgez) loop */
#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemCharUnequipBookSkills(s16 a, s16 slot) {
    s16 ret;
    s32 i;
    s32 k;
    s16 id;

    ret = -1;
    k = (slot - ptyCharDefTable[a].eggSlots) * 3;
    for (i = 0; i < 3; i++) {
        id = ptyCharData[a].skills[k + i];
        if (id != -1) {
            ptyCharData[a].skills[k + i] = -1;
            ret = 0;
            ptyPartyData->skills[id].owner = -1;
        }
    }
    ItemCharDropUnusableEquip(a);
    return ret >> 15;
}
