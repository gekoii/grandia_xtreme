#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "protos/field.h"

void ItemSpecialMoveListBuild(s16 chr) {
    s32 x10;
    PtyMove *tbl;
    s32 u;
    ItemPageSkill *e;
    s32 i;
    s32 n;
    PtyMoveLevel *s;
    s16 id;
    u8 lv;
    u8 max;
    PtyMove *sk;
    s8 x2;
    u8 x6;
    s32 xA;
    s16 r;
    PtyMoveLevelThreshold *q;
    s16 v;
    s8 c;

    tbl = ptyMoveTable;
    n = 0;
    u = ptyPartyUnits[chr].id - 0x1001;
    for (i = 0; i < 5; i++) {
        e = &itemWork->chars[chr].skills[n];
        s = &ptyPartyData->moveLevels[u * 5 + i];
        e->valid = 0;
        if (n >= 5) {
            break;
        }
        if (s->level != 0) {
            id = s->moveId;
            lv = s->level;
            sk = (PtyMove *)((id << 5) + (u32)tbl);
            x2 = sk->target;
            x6 = sk->icon;
            x10 = sk->name;
            xA = s->exp;
            max = s->maxLevel;
            r = ItemMoveGetCost(u, id);
            q = (PtyMoveLevelThreshold *)((u * 5 + i) * 24 + (u32)ptyMoveLevelThresholds);
            v = q->exp[lv];
            if (lv == max) {
                v = q->exp[max - 1];
                xA = v;
            }
            n++;
            c = ItemGetUseClass(3, q->moveId);
            e->icon = x6;
            e->unk6 = c;
            e->unk4 = r;
            e->unk7 = x2;
            e->level = lv;
            e->max = max;
            e->unkA = xA;
            e->unkC = v;
            e->valid = 1;
            e->name = x10;
            e->help = (s32)sk;
        }
    }
    itemWork->chars[chr].skillCount = n;
}
