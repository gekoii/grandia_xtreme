/* Port-only C for ItemCharSlotListBuild (0x005F50D8), which the decompilation still has in assembly. */
#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "protos/field.h"

/* itemWork->shopChrA / shopChrB as this function fills them: one record per
   egg slot (A) and book slot (B) of the character. menu.h's ItemShopChr has
   the same size but leaves most of these fields as padding. */
typedef struct {
    /* 0x00 */ s16 id;          /* spell move id (egg); the book's ptyPartyData->books index (book) */
    /* 0x02 */ u8 icon;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s32 name;
    /* 0x08 */ void *def;       /* PtyItem (egg spell) or PtySkillDef (book skill) */
    /* 0x0C */ s16 kind;        /* PtySkillDef.unk0 */
    /* 0x0E */ s16 level;
    /* 0x10 */ s16 maxLevel;
    /* 0x12 */ s16 exp;
    /* 0x14 */ s16 expNext;
    /* 0x16 */ u8 pad16[2];
} ItemCharSlotEnt; /* size 0x18 */

typedef struct {
    /* 0x00 */ s8 count;
    /* 0x01 */ u8 icon;
    /* 0x02 */ s8 slot;         /* PtyChar.slots index */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 mp;          /* egg */
    /* 0x06 */ s16 maxMp;       /* egg */
    /* 0x08 */ s16 rarity;      /* egg: PtyManaEgg.unk1A */
    /* 0x0A */ s16 bookType;    /* book: PtySkillBookType.type */
    /* 0x0C */ s16 bookSlots;   /* book: PtySkillBookType.slotCount */
    /* 0x0E */ s16 category[3]; /* book: PtySkillDef.category - 1 per skill */
    /* 0x14 */ s32 name;
    /* 0x18 */ PtyItem *item;
    /* 0x1C */ ItemCharSlotEnt ents[6];
} ItemCharSlot; /* size 0xAC */

s32 ItemCharSlotListBuild(s16 chr) {
    s32 eggSlots;
    s16 *slots;
    s16 *skills;
    s32 nEgg;
    s32 nBook;
    s32 i;
    s32 k;
    s32 cnt;
    s16 id;
    s16 x;
    s16 sid;
    ItemCharSlot *e;
    PtyItem *it;
    PtySkillBookType *bt;
    PtySkill *sk;
    PtySkillDef *d;
    s8 lv;
    s16 exp;
    s16 need;

    PtyPartyGetSlot(chr);
    eggSlots = ptyCharDefTable[chr].eggSlots;
    slots = ptyCharData[chr].slots;
    skills = ptyCharData[chr].skills;
    nBook = 0;
    nEgg = 0;
    itemWork->unk6EDE = 6 - eggSlots;
    itemWork->unk6EDC = eggSlots;
    for (i = 0; i < 6; i++) {
        id = slots[i];
        if (i < eggSlots) {
            e = (ItemCharSlot *)&itemWork->shopChrA[nEgg];
            e->slot = i;
            if (id == -1) {
                e->item = NULL;
                e->icon = 0;
                nEgg++;
                e->name = 0;
            } else {
                PtyManaEgg *egg = &ptyPartyData->eggs[id];

                e->mp = egg->mp;
                e->maxMp = egg->maxMp;
                e->rarity = egg->unk1A;
                it = &ptyItemTable[egg->itemId];
                e->name = it->name;
                e->icon = it->icon;
                e->item = it;
                cnt = 0;
                for (k = 0; k < 6; k++) {
                    x = ptyPartyData->eggs[id].spells[k];
                    if (x != 0) {
                        it = &ptyItemTable[x];
                        e->ents[cnt].id = x;
                        e->ents[cnt].icon = it->icon;
                        e->ents[cnt].name = it->name;
                        e->ents[cnt].def = it;
                        cnt++;
                    }
                }
                e->count = cnt;
                nEgg++;
            }
        } else {
            e = (ItemCharSlot *)&itemWork->shopChrB[nBook];
            e->slot = i;
            if (id == -1) {
                e->icon = 0;
                e->name = 0;
                e->item = NULL;
                for (k = 0; k < 3; k++) {
                    e->ents[k].icon = 0;
                    e->ents[k].name = 0;
                    e->ents[k].def = NULL;
                }
                e->count = 0;
            } else {
                it = &ptyItemTable[ptyPartyData->books[id].itemId];
                bt = &ptySkillBookTypeTable[it->subtype];
                e->icon = it->icon;
                e->name = it->name;
                e->item = it;
                e->bookType = bt->type;
                e->bookSlots = bt->slotCount;
                cnt = 0;
                for (k = 0; k < 3; k++) {
                    sid = skills[nBook * 3 + k];
                    if (sid == -1) {
                        /* An empty skill slot still takes an entry */
                        e->ents[cnt].icon = 0;
                        e->ents[cnt].name = 0;
                        e->ents[cnt].def = NULL;
                        cnt++;
                    } else {
                        sk = &ptyPartyData->skills[sid];
                        exp = sk->exp;
                        lv = sk->level;
                        d = &ptySkillTable[sk->id];
                        if (lv >= d->maxLevel) {
                            need = d->expNeeded[lv - 1];
                            exp = need;
                        } else {
                            need = d->expNeeded[lv];
                        }
                        e->ents[cnt].icon = d->icon;
                        e->ents[cnt].name = d->name;
                        e->ents[cnt].kind = d->unk0;
                        e->ents[cnt].level = lv;
                        e->ents[cnt].maxLevel = d->maxLevel;
                        e->ents[cnt].exp = exp;
                        e->ents[cnt].expNext = need;
                        e->ents[cnt].id = id;   /* the book's index, not the skill's */
                        e->ents[cnt].def = d;
                        e->category[cnt] = (s8)ptySkillTable[sk->id].category - 1;
                        cnt++;
                    }
                }
                e->count = cnt;
            }
            nBook++;
        }
    }
    itemWork->unk6EE0 = nEgg;
    itemWork->unk6EE2 = nBook;
    ItemEggListBuild(1);
    ItemBookListBuild(1);
    ItemCharSkillSlotListBuild(chr);
    return ItemSkillListBuild(1, itemWork->bookSlotType);
}
