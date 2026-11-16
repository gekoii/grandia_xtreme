#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/menu.h"
#include "field/n19.h"


s32 ItemSkillListBuild(s16 mode, s16 arg1) {
    s32 i;
    u16 k;

    switch (mode) {
    case 1:
        itemWork->books4Count = 0;
        break;
    case 0:
    case 2:
        itemWork->counts[3] = 0;
        break;
    }
    ItemSortSkills();
    for (i = 0; i < 100; i++) {
        k = itemWork->order[i];
        if (ptyPartyData->skills[k].id != 0) {
            switch (mode) {
            case 1:
                if (ptyPartyData->skills[k].owner == -1) {
                    if (ItemSkillRecFill(&ptyPartyData->skills[k], (ItemSkillRec *)&itemWork->books4[itemWork->books4Count], k, arg1) == 0) {
                        itemWork->books4Count++;
                    }
                }
                break;
            case 0:
                if (ptyPartyData->skills[k].owner == -1 || PtyCharIsJoined(ptyPartyData->skills[k].owner) >= 0) {
                    if (ItemSkillRecFill(&ptyPartyData->skills[k], (ItemSkillRec *)&itemWork->books2[itemWork->counts[3]], k, arg1) == 0) {
                        itemWork->counts[3]++;
                    }
                }
                break;
            case 2:
                if (ptyPartyData->skills[k].owner == -1 || PtyCharIsJoined(ptyPartyData->skills[k].owner) >= 0) {
                    if (ItemSkillRecFill(&ptyPartyData->skills[k], (ItemSkillRec *)&itemWork->books2[itemWork->counts[3]], k, arg1) == 0) {
                        itemWork->counts[3]++;
                    }
                }
                break;
            }
        }
    }
    return 0;
}
