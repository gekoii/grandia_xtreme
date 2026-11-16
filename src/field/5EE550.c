/* rodata: 0x00673BE0-0x00673D40 */
#include "common.h"
#include "party.h"
#include "field/n18.h"
#include "protos/field.h"

void DbgParamCharPage(s32 arg0, s32 sel, s32 sub, s32 dir, s32 id) {
    s32 i;
    s32 found;
    s32 x;
    s32 y;
    s32 v;
    s32 c;
    s32 row;

    found = -1;
    for (i = 0; i < 8; i++) {
        if (ptyPartyUnits[i].id - 0x1001 == id) {
            found = i;
            break;
        }
    }
    DbgDrawCursor(dbgParamX - 1 + sub * 10, dbgParamY + sel);
    x = dbgParamX;
    y = dbgParamY;
    DbgPrintf(x, y, "EXP = %d:LV %d", ptyCharData[id].exp, PtyCharCalcLevel(id));
    DbgPrintf(x, y + 1, "HP  = %3d/%3d", ptyCharData[id].hp, ptyCharData[id].maxHp);
    DbgPrintf(x, y + 2, "SP  = %3d/%3d", ptyCharData[id].sp, ptyCharData[id].maxSp);
    DbgPrintf(x, y + 4, "DEAD  = %d", ((u32)ptyCharData[id].status >> 15) & 1);
    DbgPrintf(x, y + 5, "SICK  = %d", ((u32)ptyCharData[id].status >> 11) & 1);
    DbgPrintf(x, y + 6, "POISON= %d", ((u32)ptyCharData[id].status >> 4) & 1);
    DbgPrintf(x, y + 7, "CURSE = %d", ((u32)ptyCharData[id].status >> 12) & 1);
    DbgPrintf(x, y + 10, "weapon = %4d\n", ptyCharData[id].equip[0]);
    DbgPrintf(x, y + 11, "armor  = %4d\n", ptyCharData[id].equip[1]);
    DbgPrintf(x, y + 12, "grove  = %4d\n", ptyCharData[id].equip[2]);
    DbgPrintf(x, y + 13, "hat    = %4d\n", ptyCharData[id].equip[3]);
    DbgPrintf(x, y + 14, "boots  = %4d\n", ptyCharData[id].equip[4]);
    DbgPrintf(x, y + 15, "acc    = %4d\n", ptyCharData[id].equip[5]);
    for (i = 0; i < 6; i++) {
        if (i < ptyCharDefTable[id].eggSlots) {
            v = -1;
            if (ptyCharData[id].slots[i] != -1) {
                v = ptyPartyData->eggs[ptyCharData[id].slots[i]].itemId;
            }
            c = 'E';
            row = i + 10;
            DbgPrintf(x + 17, y + i + 10, "SLOT%dE=%2d:%3d", i, ptyCharData[id].slots[i], v);
        } else {
            v = -1;
            if (ptyCharData[id].slots[i] != -1) {
                v = ptyPartyData->books[ptyCharData[id].slots[i]].itemId;
            }
            c = 'B';
            row = i + 10;
        }
        DbgPrintf(x + 17, y + row, "SLOT%d%c=%2d:%3d", i, c, ptyCharData[id].slots[i], v);
    }
    x = dbgParamX + 17;
    y = dbgParamY;
    for (i = 0; i < (6 - ptyCharDefTable[id].eggSlots) * 3; i++) {
        DbgPrintf(x, y + i, "skl%2d=B%d:%d", i, i / 3, ptyCharData[id].skills[i]);
    }
    if (found != -1) {
        x = dbgParamX + 30;
        y = dbgParamY;
        for (i = 0; i < 9; i++) {
            DbgPrintf(x, y + i, "reg %d[%f]", i, ptyPartyUnits[found].regMgcAttr[i]);
        }
    }
    if (dir == 1) {
        switch (sel) {
        case 0:
            ptyCharData[id].exp += 100000;
            if (found != -1) {
                ptyPartyUnits[found].exp = ptyCharData[id].exp;
                ptyPartyUnits[found].level = PtyCharCalcLevel(id);
            }
            break;
        case 1:
            if (sub == 0) {
                ptyCharData[id].hp += 10;
                if (found != -1) {
                    ptyPartyUnits[found].hp = ptyCharData[id].hp;
                }
            } else {
                ptyCharData[id].maxHp += 10;
                if (found != -1) {
                    ptyPartyUnits[found].maxHp = ptyCharData[id].maxHp;
                }
            }
            break;
        case 4:
            ptyCharData[id].hp = ptyCharData[id].maxHp;
            ptyCharData[id].status &= (u32)~0x8000;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
                ptyPartyUnits[found].hp = ptyCharData[id].hp;
            }
            break;
        case 5:
            ptyCharData[id].status &= (u32)~0x800;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
            }
            break;
        case 6:
            ptyCharData[id].status &= (u32)~0x10;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
            }
            break;
        case 7:
            ptyCharData[id].status &= (u32)~0x1000;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
            }
            break;
        }
    }
    if (dir == -1) {
        switch (sel) {
        case 0:
            ptyCharData[id].exp -= 10000;
            if (found != -1) {
                ptyPartyUnits[found].exp = ptyCharData[id].exp;
                ptyPartyUnits[found].level = PtyCharCalcLevel(id);
            }
            break;
        case 1:
            if (sub == 0) {
                ptyCharData[id].hp -= 10;
                if (found != -1) {
                    ptyPartyUnits[found].hp = ptyCharData[id].hp;
                }
            } else {
                ptyCharData[id].maxHp -= 10;
                if (found != -1) {
                    ptyPartyUnits[found].maxHp = ptyCharData[id].maxHp;
                }
            }
            break;
        case 4:
            ptyCharData[id].hp = 0;
            ptyCharData[id].status |= 0x8000;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
                ptyPartyUnits[found].hp = ptyCharData[id].hp;
            }
            break;
        case 5:
            ptyCharData[id].status |= 0x800;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
            }
            break;
        case 6:
            ptyCharData[id].status |= 0x10;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
            }
            break;
        case 7:
            ptyCharData[id].status |= 0x1000;
            if (found != -1) {
                ptyPartyUnits[found].status = ptyCharData[id].status;
            }
            break;
        }
    }
}
