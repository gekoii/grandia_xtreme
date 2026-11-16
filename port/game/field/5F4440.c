/* Port-only C for ItemEggSpellRecFill (0x005F4440), which the decompilation still has in assembly. */
#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "protos/field.h"

extern const s8 itemEggUpgradeIdTable[7];

/* The layout ItemEggSpellRecFill writes; field/menu.h's ItemEggSpellRec only
   has +0 so far. */
typedef struct EggSpell {
    /* 0x00 */ u8 moveId;          /* stored as a byte */
    /* 0x01 */ u8 icon;            /* PtyMove.icon */
    /* 0x02 */ s16 cost;           /* by mode: PtyManaEggSpellCost, levels->mpCost or 0 */
    /* 0x04 */ s8 target;          /* PtyMove.target */
    /* 0x05 */ u8 useClass;        /* ItemGetUseClass(3, move) */
    /* 0x06 */ u8 upgrades[6];     /* itemEggUpgradeIdTable ids of the set bits, up to 5 */
    /* 0x0C */ s32 name;           /* PtyMove.name */
    /* 0x10 */ PtyMove *move;
} EggSpell; /* size 0x14 */

typedef struct EggSpellRec {
    /* 0x00 */ s16 itemId;
    /* 0x02 */ s8 owner;
    /* 0x03 */ u8 icon;            /* PtyItem.icon */
    /* 0x04 */ u8 spellCount;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 name;           /* PtyItem.name */
    /* 0x0C */ PtyItem *item;
    /* 0x10 */ s16 mp;
    /* 0x12 */ s16 maxMp;
    /* 0x14 */ s16 rarity;         /* PtyManaEgg.unk1A */
    /* 0x16 */ s16 who;
    /* 0x18 */ EggSpell spells[6];
    /* 0x90 */ u8 unk90;
    /* 0x91 */ u8 pad91[3];
    /* 0x94 */ s32 unk94;
} EggSpellRec; /* size 0x98 */

void ItemEggSpellRecFill(PtyManaEgg *egg, ItemEggSpellRec *out, s16 mode, s16 who) {
    EggSpellRec *rec = (EggSpellRec *)out;
    s8 ids[7];
    PtyItem *item;
    PtyMove *move;
    EggSpell *sp;
    s32 owner;
    s16 itemId;
    s16 moveId;
    s32 cost;
    s32 n;
    s32 cnt;
    s32 i;
    s32 j;
    u8 bits;

    owner = egg->owner;
    itemId = egg->itemId;
    for (i = 0; i < 7; i++) {
        ids[i] = itemEggUpgradeIdTable[i];
    }
    item = &ptyItemTable[itemId];
    ItemGetUseClass(1, itemId); /* result unused */
    rec->icon = item->icon;
    rec->item = item;
    rec->name = item->name;
    rec->itemId = itemId;
    rec->mp = egg->mp;
    rec->maxMp = egg->maxMp;
    rec->rarity = egg->unk1A;
    rec->owner = owner;
    rec->unk90 = 0;
    rec->who = who;
    rec->unk94 = 0;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            rec->spells[i].upgrades[j] = 0;
        }
    }

    n = 0;
    for (i = 0; i < 6; i++) {
        moveId = egg->spells[i];
        if (moveId == 0) {
            continue;
        }
        move = &ptyMoveTable[moveId];
        sp = &rec->spells[i];
        cost = 0;
        bits = egg->spellUpgrades[i];
        sp->icon = move->icon;
        sp->moveId = moveId;
        switch (mode) {
        case 0:
        case 1:
            cost = PtyManaEggSpellCost(&ptyPartyUnits[PtyPartyGetSlot(owner)], who, i);
            break;
        case 2:
        case 4:
            cost = ptyMoveTable[moveId].levels->mpCost;
            break;
        }
        sp->useClass = ItemGetUseClass(3, moveId);
        sp->cost = cost;
        sp->target = move->target;
        cnt = 0;
        for (j = 0; j < 7 && cnt < 5; j++) {
            if ((bits >> j) & 1) {
                sp->upgrades[cnt++] = ids[j];
            }
        }
        n++;
        sp->name = move->name;
        sp->move = move;
    }
    rec->spellCount = n;
}
