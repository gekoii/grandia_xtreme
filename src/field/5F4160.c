#include "common.h"
#include "party.h"
#include "field/n19.h"
#include "protos/field.h"

void ItemEggRecFill(PtyManaEgg *src, ItemEggListRec *dst, s16 arg2) {
    s16 id = src->itemId;
    s8 c = src->unk1A;
    s16 d = src->maxMp;
    s16 e = src->mp;
    s8 b = src->owner;
    PtyItem *def;
    u8 k;
    s32 m;

    dst->useClass = ItemGetUseClass(1, id);
    def = (PtyItem *)((id << 5) + (u32)ptyItemTable);
    k = def->icon;
    m = def->name;
    dst->index = arg2;
    dst->itemId = id;
    dst->owner = b;
    dst->rarity = c;
    dst->maxMp = d;
    dst->mp = e;
    dst->icon = k;
    dst->name = m;
    dst->item = def;
    dst->unk3 = 0;
}
