#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "party.h"


void PtyCharInit(s16 arg0, SysIniParty *arg1) {
    s32 idx;
    PtyChar *out;
    PtyCharDef *def;
    PtyGrowth *lvl;
    s32 i;
    s32 j;
    s32 k;
    s32 lv;

    idx = arg0 - 0x1001;
    out = &ptyCharData[idx];
    def = &ptyCharDefTable[idx];
    out->exp = arg1 == NULL ? def->initExp : arg1->exp;
    out->maxSp = def->initMaxSp;
    out->maxHp = def->initMaxHp;
    out->stats[0] = def->initStats[0];
    out->status = 0;
    out->hpGrowth = 0;
    out->stats[1] = def->initStats[1];
    out->stats[2] = def->initStats[2];
    out->stats[3] = def->initStats[3];
    out->stats[4] = def->initStats[4];
    out->stats[5] = def->initStats[5];
    out->spGrowth = 0;
    out->statGrowth[0] = 0;
    out->statGrowth[1] = 0;
    out->statGrowth[2] = 0;
    out->statGrowth[3] = 0;
    out->statGrowth[4] = 0;
    out->statGrowth[5] = 0;
    for (j = 0; j < 6; j++) {
        out->equip[j] = arg1 == NULL ? def->initEquip[j] : arg1->equip[j];
    }
    for (j = 0; j < 6; j++) {
        out->slots[j] = -(arg1 == NULL ? def->initSlots[j] : arg1->slot[j]);
        if (out->slots[j] >= 0) {
            out->slots[j] = -1;
        }
    }
    for (j = 0; j < 15; j++) {
        out->skills[j] = arg1 == NULL ? def->initSkills[j] : arg1->skills[j];
        if (out->skills[j] <= 0) {
            out->skills[j] = -1;
        }
    }
    for (i = 0; i < 6; i++) {
        if (out->slots[i] < 0) {
            if (out->slots[i] == -1) {
                continue;
            }
            switch (ptyItemTable[-out->slots[i]].type) {
            case 11:
                out->slots[i] = PtyManaEggAdd(-out->slots[i]);
                if (out->slots[i] >= 0) {
                    ptyPartyData->eggs[out->slots[i]].owner = idx;
                }
                break;
            case 12:
                out->slots[i] = PtySkillBookAdd(-out->slots[i]);
                if (out->slots[i] >= 0) {
                    ptyPartyData->books[out->slots[i]].owner = idx;
                }
                break;
            default:
                out->slots[i] = -1;
                break;
            }
        }
    }
    for (i = 0; i < 15; i++) {
        if (out->skills[i] > 0) {
            out->skills[i] = PtySkillAdd(out->skills[i]);
            if (out->skills[i] >= 0) {
                ptyPartyData->skills[out->skills[i]].owner = idx;
            }
        } else {
            out->skills[i] = -1;
        }
    }
    for (k = 0; k < 2; k++) {
        out->tactics[k] = k + 1;
    }
    out->battleCount = 0;
    out->kills = 0;
    out->killMax = 0;
    out->killTotal = 0;
    lvl = &ptyGrowthTable[idx];
    out->deaths = 0;
    out->mvp = 0;
    for (lv = 1; lv < 250 && out->exp >= lvl->exp[lv]; lv++) {
        PtyCharLevelUpStats(idx, lv + 1, 0);
    }
    out->sp = 0;
    out->hp = out->maxHp;
}
