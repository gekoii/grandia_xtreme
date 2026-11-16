#include "common.h"
#include "field/n19.h"
#include "party.h"
#include "protos/field.h"

s32 ItemBookRecFill(PtySkillBook *s, ItemBookRec *it, s16 idx) {
    s16 id;
    s8 o;
    PtyItem *e;
    PtySkillBookType *t;
    u8 x1;
    s32 xC;
    s8 a;
    s8 b;
    s16 c;
    s32 i;
    s32 j;
    s32 base;
    s16 k;

    id = s->itemId;
    o = s->owner;
    e = (PtyItem *)((id << 5) + (u32)ptyItemTable);
    x1 = e->icon;
    xC = e->name;
    t = (PtySkillBookType *)((e->subtype << 2) + (u32)ptySkillBookTypeTable);
    a = t->type;
    b = t->slotCount;
    c = t->value;
    it->useClass = ItemGetUseClass(2, id);
    it->itemId = id;
    it->icon = x1;
    it->type = a;
    it->slotCount = b;
    it->name = xC;
    it->item = e;
    it->value = c;
    it->owner = o;
    it->index = idx;
    if (o != -1) {
        PtyPartyGetSlot(o);
        base = ptyCharDefTable[o].eggSlots;
        for (i = 0; i < 3; i++) {
            it->skillOwners[i] = -1;
            it->skillIcons[i] = -1;
        }
        for (i = 0; i < 6 - base; i++) {
            if (ptyCharData[o].slots[base + i] == idx) {
                for (j = 0; j < 3; j++) {
                    k = ptyCharData[o].skills[i * 3 + j];
                    if (k != -1) {
                        it->skillOwners[j] = o;
                        it->skillIcons[j] = ptySkillTable[ptyPartyData->skills[k].id].icon;
                    }
                }
                break;
            }
        }
    } else {
        for (i = 0; i < 3; i++) {
            it->skillOwners[i] = -1;
            it->skillIcons[i] = -1;
        }
    }
    return 0;
}
