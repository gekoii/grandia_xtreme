/* rodata: 0x00674048-0x0067406C */
#include "common.h"
#include "field/menu.h"
#include "field/n19.h"
#include "party.h"
#include "protos/field.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    u8 unk3[7];
    u8 padA[2];
    s32 unkC;
} Fuse;

extern Fuse *sysCombineTable;

void ItemEggSpellRecFill(PtyManaEgg *, ItemEggSpellRec *, s16, s16);

s32 ItemEggFusePreview(s16 x, s16 y) {
    s16 ids[3];
    s16 kinds[2];
    s16 lv[3];
    s16 stats[7];
    u8 tblA[9] = { 0, 1, 2, 1, 3, 4, 2, 4, 5 };
    u8 tblB[6][3] = { { 90, 99, 100 }, { 60, 90, 100 }, { 60, 70, 100 }, { 60, 100, 100 }, { 60, 80, 100 }, { 60, 60, 40 } };
    s32 t;
    Fuse *rec;
    PtyManaEgg *n;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 q;
    s32 r;
    s32 c;
    s8 res;

    ids[0] = ptyPartyData->eggs[x].itemId;
    kinds[0] = (s8)ptyPartyData->eggs[x].unk1A;
    lv[0] = ptyPartyData->eggs[x].maxMp;
    ids[1] = ptyPartyData->eggs[y].itemId;
    kinds[1] = (s8)ptyPartyData->eggs[y].unk1A;
    lv[1] = ptyPartyData->eggs[y].maxMp;
    n = &itemWork->u6E20.eggRecs[0];
    m = (ids[1] - 0x12D) * 0x2C - 0x12D;
    t = ptyEggFusionTable->result[ids[0] + m];
    rec = &sysCombineTable[t];
    ids[2] = n->itemId = (s8)rec->unk1 + 300;
    lv[2] = (lv[0] + lv[1]) * (rec->unk2 / 100.0);
    if (lv[2] >= 1000) {
        lv[2] = 999;
    }
    n->type = rec->unk1;
    n->owner = -1;
    n->mp = n->maxMp = lv[2];
    k = 0;
    for (q = 0; q < 2; q++) {
        if (kinds[q] == 1) {
            k += 15;
        }
        if (kinds[q] == 2) {
            k += 30;
        }
    }
    for (j = 0; j < 7; j++) {
        stats[j] = (s8)rec->unk3[j] + k;
    }
    for (i = 0; i < 6; i++) {
        n->spells[i] = ptyManaEggTable[n->type].spells[i];
        n->spellUpgrades[i] = 0;
        if ((s16)n->spells[i] != 0) {
            for (j = 0; j < 7; j++) {
                c = ptyManaEggTable[n->type].upgradeChance[i][j];
                if (c != -1) {
                    if ((s32)VuRandInt(101) < c + stats[j]) {
                        n->spellUpgrades[i] |= 1 << j;
                    }
                }
            }
        }
    }
    c = tblA[kinds[1] + kinds[0] * 3];
    r = VuRandInt(101);
    res = 0;
    if (r >= tblB[c][0]) {
        res = 2;
        if (r < tblB[c][1]) {
            res = 1;
        }
    }
    n->unk1A = res;
    if (n->itemId - 300 == 28) {
        if ((s8)n->unk1A == 1) {
            n->type = 29;
            n->itemId = 329;
            n->unk1A = 1;
        }
        if ((s8)n->unk1A == 2) {
            n->type = 30;
            n->itemId = 330;
            n->unk1A = 2;
        }
    }
    ItemEggSpellRecFill(&itemWork->u6E20.eggRecs[0], (ItemEggSpellRec *)&itemWork->u6E20.eggRecs[1], 2, 0);
    itemWork->unk6ED0 = rec->unkC;
    if (t == 0) {
        itemWork->unk6ECC = 2;
        return -1;
    }
    return 0;
}
