#include "common.h"
#include "party.h"
#include "field/menu.h"

void ItemSkillShopRecFill(ItemBagRec *r, u16 id, s16 arg2) {
    PtySkillDef *d = (PtySkillDef *)(id * 0x2C + (u32)ptySkillTable);
    u8 b = d->icon;
    s32 c = d->name;
    PtyItem *p = &ptyItemTable[0x2BE];
    r->id = id;
    r->unk2 = b;
    r->unkC = c;
    r->unk10 = p;
    r->unk8 = 2;
    r->unkA = arg2;
    r->unk5 = 1;
    r->unk4 = 0;
}
