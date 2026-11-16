#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "protos/field.h"

void ItemComboListBuild(s16 chr) {
    s32 off;
    s32 x4;
    s32 x5;
    s32 u;
    ItemPageMove *e;
    PtyMoveLevel *p;
    s32 i;
    s32 j;
    s32 n;
    s32 found;
    s16 id;
    PtyMove *sk;
    s8 c;
    s8 x2;
    u8 x6;
    s32 x10;
    s16 r;

    u = ptyPartyUnits[chr].id - 0x1001;
    n = 0;
    for (i = 0; i < 50; i++) {
        e = &itemWork->chars[chr].moves[n];
        p = &ptyPartyData->moveLevels[PTY_MOVE_LEVEL_MOVES + i];
        if (n >= 25) {
            break;
        }
        id = p->moveId;
        x4 = p->level;
        x5 = p->maxLevel;
        off = id << 5;
        found = 0;
        for (j = 0; j < 4; j++) {
            if (ptyMoveTable[id].members[j] == u + 1) {
                found = 1;
                break;
            }
        }
        if (found) {
            c = ((s8 (*)(s16, s16))ItemGetUseClass)(3, ptyComboTable[i].moveId); /* s8 cast: the u8 return changes the code */
            sk = (PtyMove *)(off + (u32)ptyMoveTable);
            x2 = sk->target;
            x6 = sk->icon;
            x10 = sk->name;
            r = ItemMoveGetCost(u, id);
            e->valid = 0;
            if (p->level != 0) {
                e->valid = 1;
            }
            e->icon = x6;
            e->unk6 = c;
            e->unk4 = r;
            e->unk7 = x2;
            e->level = x4;
            e->max = x5;
            e->name = x10;
            e->help = (s32)sk;
            for (j = 0; j < 4; j++) {
                e->slot[j][0] = ptyMoveTable[id].members[j];
                e->slot[j][1] = ptyMoveTable[id].members[j];
            }
            n++;
        }
    }
    itemWork->chars[chr].unk46 = n;
}
