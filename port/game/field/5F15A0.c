/* Port-only C for ItemSkillItemListBuild (0x005F15A0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/menu.h"

/*
 * Lists the skill items in the bag (item type 13) into itemWork->sale[]
 * (the skill shop's records), in sort order, then counts the learned
 * skills per category into itemWork +0x1106 (u16[4], inside pad1106).
 */
void ItemSkillItemListBuild(void) {
    s32 i;
    s32 n;
    u16 ord;
    s16 id;
    s8 count;
    PtyItem *item;
    PtySkillDef *skill;
    ItemSaleRec *rec;
    u8 sid;
    u16 *catCounts;

    ItemSortInventory();
    n = 0;
    for (i = 0; i < 30; i++) {
        ord = itemWork->order[i];
        id = ptyPartyData->bag[ord].id;
        if (id == 0) {
            continue;
        }
        count = ptyPartyData->bag[ord].count;
        if (count == 0) {
            continue;
        }
        item = &ptyItemTable[id];
        if (item->type != 13) {
            continue;
        }
        rec = &itemWork->sale[n];
        n++;
        /* +0 item id, +2 count: pad0 in ItemSaleRec */
        rec->pad0[2] = count;
        *(s16 *)&rec->pad0[0] = id;
        rec->icon = item->icon;
        rec->name = item->name;
        rec->help = (s32)item;
        rec->unk4 = (u8)ord;
        sid = ptyItemTable[id].subtype;
        skill = &ptySkillTable[sid];
        rec->price = skill->unk4;
        rec->icon2 = skill->icon;
        rec->id = sid;
        rec->desc = skill->name;
        rec->msg = (s32)skill;
    }

    catCounts = (u16 *)&itemWork->pad1106[0];
    for (i = 3; i >= 0; i--) {
        catCounts[i] = 0;
    }
    for (i = 0; i < PTY_SKILL_SLOTS; i++) {
        sid = ptyPartyData->skills[i].id;
        if (sid != 0) {
            catCounts[ptySkillTable[sid].category - 1]++;
        }
    }
    itemWork->saleCount = n;
}
