#include "common.h"
#include "party.h"
#include "field/menu.h"

void ItemCharSkillSlotListBuild(s16 arg0) {
    s32 base;
    s32 i;
    s32 j;
    s32 cnt;
    s32 s;
    s32 n;
    s16 id;
    s16 e;
    s16 sid;
    ItemRec *o;
    PtySkill *sl;
    PtySkillDef *d;
    u8 k;
    s8 lv;
    s16 a;
    s16 b;

    cnt = 0;
    base = ptyCharDefTable[arg0].eggSlots;
    for (i = 0; i < 6 - base; i++) {
        s = base + i;
        id = ptyCharData[arg0].slots[s];
        if (id == -1) {
            continue;
        }
        e = ptyPartyData->books[id].itemId;
        if (e == -1) {
            continue;
        }
        n = ptySkillBookTypeTable[ptyItemTable[e].subtype].slotCount;
        for (j = 0; j < n; j++) {
            o = &itemWork->u76F8.recs[cnt];
            sid = ptyCharData[arg0].skills[i * 3 + j];
            if (sid == -1) {
                o->name = 0;
                itemWork->u76F8.recs[cnt].unk0 = 0;
                o->help = 0;
            } else {
                sl = &ptyPartyData->skills[sid];
                k = sl->id;
                lv = sl->level;
                d = (PtySkillDef *)(k * 0x2C + (u32)ptySkillTable);
                if (lv >= d->maxLevel) {
                    b = d->expNeeded[lv - 1];
                    a = b;
                } else {
                    a = sl->exp;
                    b = d->expNeeded[lv];
                }
                o->unk0 = k;
                o->icon = ptySkillTable[k].icon;
                o->type = ptySkillTable[k].unk0;
                o->max = ptySkillTable[k].maxLevel;
                d = (PtySkillDef *)(k * 0x2C + (u32)ptySkillTable);
                *(PtySkillDef **)&o->help = d; /* stored as a pointer: an s32 store schedules differently (the readers need help as s32) */
                o->name = d->name;
                o->unk3 = sl->owner;
                o->n = lv;
                o->unkC = a;
                o->unkE = b;
                o->price = 0;
            }
            o->unk8 = j;
            cnt++;
            o->owner = i;
            o->id = s;
        }
    }
    itemWork->unk76F4 = cnt;
}
