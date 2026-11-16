#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/menu.h"

s32 ItemShopCondCheck(void *);

s16 ItemShopEntryBuild(s16 cmd, s16 src, s16 id, s16 cnt, s16 slot, void *cond, s16 arg6) {
    s32 sub = 0;
    s32 kind = 0;
    s32 r;
    s32 mask = 0;
    ItemShopRec *base = itemWork->shop;
    s32 i;
    PtyItem *d;
    s32 v;

    switch (src) {
    case 0:
        d = (PtyItem *)((id << 5) + (u32)ptyItemTable);
        v = d->type;
        if (v == 0) {
            return -1;
        }
        kind = v;
        sub = d->icon;
        if (d->equip != NULL) {
            mask = d->equip->charMask;
            if (kind != 5) {
                for (i = 0; i < 8; i++) {
                    if (ItemCharHasEquipAllSkill(i)) {
                        mask |= 1 << i;
                    }
                }
            }
        }
        break;
    case 1:
        sub = ptySkillTable[id].icon;
        break;
    }
    r = -1;
    if ((u16)(cmd - 30) < 21) {
        if (!ItemShopCondCheck(cond)) {
            return -1;
        }
        if (!ItemIsShopSeen(id)) {
            ItemSetShopSeen(id);
            itemWork->shopNew[id] = 1;
        }
    }
    switch (cmd) {
    case 0:
        if (kind == 5) {
            if ((u16)(id - 701) <= 99) {
                r = 1;
            }
        }
        break;
    case 1:
        if (kind == 5) {
            if ((u16)(id - 801) <= 99) {
                r = 1;
            }
        }
        break;
    case 2:
        if (kind == 5) {
            if ((u16)(id - 901) <= 99) {
                r = 1;
            }
        }
        break;
    case 3:
        if (kind == 5) {
            if ((u16)(id - 1001) <= 99) {
                r = 1;
            }
        }
        break;
    case 4:
        if (kind == 5) {
            if ((u16)(id - 1101) <= 99) {
                r = 1;
            }
        }
        break;
    case 5:
        if (kind == 5) {
            if ((u16)(id - 1201) <= 99) {
                r = 1;
            }
        }
        break;
    case 6:
        if (kind == 5) {
            if ((u16)(id - 1301) <= 99) {
                r = 1;
            }
        }
        break;
    case 8:
        if (kind == 6) {
            if ((sub == 30 || sub == 31)) {
                r = 2;
            }
        }
        break;
    case 9:
        if (kind == 6) {
            if ((sub != 30 && sub != 31)) {
                r = 2;
            }
        }
        break;
    case 11:
        if (kind == 7) {
            if ((sub == 37 || sub == 38)) {
                r = 2;
            }
        }
        break;
    case 16:
        if (kind == 13) {
            r = 0;
        }
        break;
    case 17:
        if (kind == 10) {
            r = ItemGetEquipCategory(id);
        }
        break;
    case 18:
        if (!(id >= 201 && id <= 300)) {
            if (kind == 3) {
                r = 0;
            }
        }
        break;
    case 19:
        if ((u16)(id - 201) <= 99) {
            r = 0;
        }
        break;
    case 20:
        if (kind == 4) {
            r = 0;
        }
        break;
    case 23:
    case 24:
        r = 0;
        break;
    case 30:
        if (kind == 5) {
            if ((u16)(id - 701) <= 99) {
                r = 1;
            }
        }
        break;
    case 31:
        if (kind == 5) {
            if ((u16)(id - 801) <= 99) {
                r = 1;
            }
        }
        break;
    case 32:
        if (kind == 5) {
            if ((u16)(id - 901) <= 99) {
                r = 1;
            }
        }
        break;
    case 33:
        if (kind == 5) {
            if ((u16)(id - 1001) <= 99) {
                r = 1;
            }
        }
        break;
    case 34:
        if (kind == 5) {
            if ((u16)(id - 1101) <= 99) {
                r = 1;
            }
        }
        break;
    case 35:
        if (kind == 5) {
            if ((u16)(id - 1201) <= 99) {
                r = 1;
            }
        }
        break;
    case 36:
        if (kind == 5) {
            if ((u16)(id - 1301) <= 99) {
                r = 1;
            }
        }
        break;
    case 7:
    case 37:
        if (kind == 5) {
            if ((u16)(id - 1401) <= 99) {
                r = 1;
            }
        }
        break;
    case 38:
        if (kind == 6) {
            if ((sub == 30 || sub == 31)) {
                r = 2;
            }
        }
        break;
    case 39:
        if (kind == 6) {
            if ((sub != 30 && sub != 31)) {
                r = 2;
            }
        }
        break;
    case 10:
    case 40:
        if (kind == 7) {
            if ((sub >= 34 && sub <= 36)) {
                r = 2;
            }
        }
        break;
    case 41:
        if (kind == 7) {
            if ((sub == 37 || sub == 38)) {
                r = 2;
            }
        }
        break;
    case 12:
    case 42:
        if (kind == 8) {
            if ((sub == 39 || sub == 40)) {
                r = 2;
            }
        }
        break;
    case 13:
    case 43:
        if (kind == 8) {
            if ((sub != 39 && sub != 40)) {
                r = 2;
            }
        }
        break;
    case 14:
    case 44:
        do { /* do-while(0) (and case 15's): needed for register allocation */
            if (kind == 9) {
                if (sub == 44) {
                    r = 2;
                }
            }
        } while (0);
        break;
    case 15:
    case 45:
        do {
            if (kind == 9) {
                if (sub != 44) {
                    r = 6;
                }
            }
        } while (0);
        break;
    case 46:
        if ((u16)arg6 <= 15) {
            r = 0;
        }
        break;
    case 47:
        if (arg6 >= 16 && arg6 <= 31) {
            r = ItemGetEquipCategory(id);
        }
        break;
    case 21:
    case 22:
    case 25:
        r = ItemGetEquipCategory(id);
        break;
    case 48:
        if ((u16)(arg6 - 32) <= 15) {
            r = 0;
        }
        break;
    case 49:
        if ((u16)(arg6 - 48) <= 15) {
            r = 0;
        }
        break;
    case 50:
        if ((u16)(arg6 - 64) <= 15) {
            r = 0;
        }
        break;
    }
    if (r < 0) {
        return -1;
    }
    base[slot].count = cnt;
    base[slot].mask = mask;
    switch (src) {
    case 0:
        ItemInfoFill((ItemBagRec *)&base[slot], id, arg6);
        break;
    case 1:
        ItemSkillShopRecFill((ItemBagRec *)&base[slot], id, arg6);
        break;
    }
    base[slot].unk9 = 0;
    return r;
}
