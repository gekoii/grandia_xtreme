/* rodata: 0x00673F20-0x00673F30 */
#include "common.h"
#include "party.h"
#include "field/n19.h"

s32 ItemSkillRecFill(PtySkill *s, ItemSkillRec *it, s16 idx, s16 filt) {
    s8 x2;
    s8 b;
    s8 x0;
    s32 x4;
    PtySkillDef *d;
    s8 x3;
    s32 ok;
    s16 v;
    s32 n;
    u8 a;
    s16 e;
    s32 x20;
    s8 lv;

    a = s->id;
    b = s->owner;
    lv = s->level;
    d = (PtySkillDef *)(a * 0x2C + (u32)ptySkillTable);
    x2 = d->icon;
    e = s->exp;
    x3 = d->maxLevel;
    x0 = d->unk0;
    x20 = d->name;
    x4 = d->unk4;
    if (lv >= x3) {
        v = d->expNeeded[lv - 1];
        e = v;
    } else {
        v = d->expNeeded[lv];
    }
    if (filt == -1) {
        ok = 1;
    } else if (filt != 0) {
        ok = x0 == filt;
    } else {
        ok = 1;
    }
    if (!ok) {
        return -1;
    }
    if (x0) {
        n = x4 * 0.1 * (lv + 1);
    } else {
        n = x4 * 0.2 * (lv + 1);
    }
    if (ptySkillTable[a].category == 4) {
        n *= 2;
    }
    it->skillId = a;
    it->icon = x2;
    it->owner = b;
    it->kind = x0;
    it->level = lv;
    it->maxLevel = x3;
    it->exp = e;
    it->expNext = v;
    it->name = x20;
    it->def = d;
    it->amount = n;
    it->index = idx;
    return 0;
}
