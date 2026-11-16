/* rodata: 0x006738B8-0x006738D0 */
#include "common.h"
#include "protos/main.h"
#include "protos/field.h"
#include "party.h"
#include "grxsys.h"
#include "field/menu.h"

void MemSet(void *, s32, s32);
void ItemGetEquipStats(s16, u16 *);

/* Accessors for the GRXSYS shop stock records (0xA bytes, 150 per list). */
static inline s16 rd16(u32 b, s32 o) { return *(s16 *)(b + o); }
static inline u16 rdu16(u32 b, s32 o) { return *(u16 *)(b + o); }

typedef struct ShopBonus {
    s16 id;
    u8 pad2[2];
    s32 price;
} ShopBonus; /* sysGrxTable7C records */

void ItemShopListBuild(s16 arg0) {
    s16 stats[8][6];
    s16 diff[8][6];
    u16 base6[8][6];
    u16 cur[6];
    u16 own[6];
    s32 params[4];
    s8 groups[21] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
    s32 cnt;
    s32 grp;
    s32 val;
    ItemShopRec *ent;
    s32 i;
    s32 j;
    s32 k = 0;
    s32 m = 0;
    s32 r = 0;
    s32 x = 0;
    s16 id;
    u16 t;
    s16 id2;
    u8 *p2;
    s32 v;

    ent = itemWork->shop;
    for (i = 0; i < 200; i++) {
        for (j = 0; j < 8; j++) {
            ent[i].stats[j] = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        MemSet(&ptyCharCalcUnits[i], 0, sizeof(PtyUnit));
        PtyUnitInitFromChar(&ptyCharCalcUnits[i], i + 0x1001);
        PtyUnitCalcParams(&ptyCharCalcUnits[i], 0, 0);
        for (j = 0; j < 6; j++) {
            stats[i][j] = ptyCharData[i].equip[j];
        }
        base6[i][0] = ptyCharCalcUnits[i].params[0];
        base6[i][1] = ptyCharCalcUnits[i].params[1];
        base6[i][2] = ptyCharCalcUnits[i].params[2];
        base6[i][3] = ptyCharCalcUnits[i].params[3];
        base6[i][4] = ptyCharCalcUnits[i].params[4];
        base6[i][5] = ptyCharCalcUnits[i].params[5];
    }
    cnt = 0;
    if (arg0 < 0x15) {
        ItemSortAllItems();
        for (i = 0; i < 0x800; i++) {
            t = itemWork->order[i];
            if (cnt >= 0xC9) break;
            j = *(s8 *)((u8 *)ptyPartyData + t + 0xFA); /* storage[t]; the indexed form changes PRE order */
            if (j != 0) {
                id = t;
                r = ItemShopEntryBuild(arg0, 0, id, j, cnt, params, t);
                if (r >= 0) {
                    ItemGetEquipStats(id, own);
                    k = ItemGetEquipSlot(id);
                    for (j = 0; j < 8; j++) {
                        ItemGetEquipStats(stats[j][k], cur);
                        for (m = 0; m < 6; m++) {
                            diff[j][m] = own[m] - cur[m];
                        }
                        if (r != 0) {
                            ent[cnt].stats[j] = diff[j][r - 1];
                        } else {
                            ent[cnt].stats[j] = 0;
                        }
                    }
                    ent[cnt].unk19 = r;
                    cnt++;
                }
            }
        }
    }
    if ((u16)(arg0 - 0x15) < 2) {
        ItemSortInventory();
        for (i = 0; i < 0x1E; i++) {
            t = itemWork->order[i];
            if (cnt >= 0xC9) break;
            v = ptyPartyData->bag[t].id;
            if (v != 0 && ptyPartyData->bag[t].count != 0) {
                x = v;
                j = ptyPartyData->bag[t].count;
                {
                if (ptyItemTable[x].type != 0xE) {
                    id = x;
                    r = ItemShopEntryBuild(arg0, 0, id, j, cnt, params, t);
                    if (r >= 0) {
                        ItemGetEquipStats(id, own);
                        k = ItemGetEquipSlot(id);
                        for (j = 0; j < 8; j++) {
                            ItemGetEquipStats(stats[j][k], cur);
                            for (m = 0; m < 6; m++) {
                                diff[j][m] = own[m] - cur[m];
                            }
                            if (r != 0) {
                                ent[cnt].stats[j] = diff[j][r - 1];
                            } else {
                                ent[cnt].stats[j] = 0;
                            }
                        }
                        ent[cnt].unk19 = r;
                        ent[cnt].price = (u32)ent[cnt].price >> 1;
                        cnt++;
                    }
                }
                }
            }
        }
    }
    if (arg0 == 0x17) {
        ItemSortSkills();
        for (i = 0; i < 0x64; i++) {
            t = itemWork->order[i];
            if (cnt >= 0xC9) break;
            p2 = (u8 *)ptyPartyData + t * 8;
            if (((PtySkill *)(p2 + 0x115A))->id != 0 && ((PtySkill *)(p2 + 0x115A))->owner == 0) {
                if (ItemShopEntryBuild(arg0, 1, ((PtySkill *)(p2 + 0x115A))->id, 1, cnt, params, t) >= 0) {
                    cnt++;
                }
            }
        }
    }
    if (arg0 == 0x18) {
        ItemSortBooks();
        for (i = 0; i < 0x18; i++) {
            t = itemWork->order[i];
            if (cnt >= 0xC9) break;
            if (ptyPartyData->books[t].itemId != 0 && ptyPartyData->books[t].owner == 0) {
                if (ItemShopEntryBuild(arg0, 0, ptyPartyData->books[t].itemId, 1, cnt, params, t) >= 0) {
                    cnt++;
                }
            }
        }
    }
    if (arg0 == 0x19) {
        for (i = 0; i < 0x10 && cnt < 0xC9; i++) {
            val = ((ShopBonus *)sysGrxTable7C)[i].price;
            id2 = ((ShopBonus *)sysGrxTable7C)[i].id;
            r = ItemShopEntryBuild(arg0, 0, id2, 1, cnt, params, i);
            if (r >= 0) {
                ItemGetEquipStats(id2, own);
                k = ItemGetEquipSlot(id2);
                for (j = 0; j < 8; j++) {
                    ItemGetEquipStats(stats[j][k], cur);
                    for (m = 0; m < 6; m++) {
                        diff[j][m] = own[m] - cur[m];
                    }
                    if (r != 0) {
                        ent[cnt].stats[j] = diff[j][r - 1];
                    } else {
                        ent[cnt].stats[j] = 0;
                    }
                }
                ent[cnt].unk19 = r;
                ent[cnt].price = val;
                cnt++;
            }
        }
    }
    if ((u16)(arg0 - 0x1E) < 0x15) {
        grp = groups[arg0 - 0x1E];
        for (i = 0; i < 0x96 && cnt < 0xC9; i++) {
            x = rd16(((u32)sysGrxTableA0 + grp * 1500), i * 10);
            params[0] = rd16(((u32)sysGrxTableA0 + grp * 1500) + 2, i * 10);
            v = rd16(((u32)sysGrxTableA0 + grp * 1500) + 4, i * 10); params[1] = v;
            params[2] = rd16(((u32)sysGrxTableA0 + grp * 1500) + 6, i * 10);
            params[3] = rdu16(((u32)sysGrxTableA0 + grp * 1500) + 8, i * 10);
            id = x;
            j = 1;
            r = ItemShopEntryBuild(arg0, 0, id, j, cnt, params, i);
            if (r >= 0) {
                ItemGetEquipStats(id, own);
                k = ItemGetEquipSlot(id);
                ent[cnt].unk9 = 0;
                if (itemWork->shopNew[x] == 1) {
                    ent[cnt].unk9 = itemWork->shopNew[x];
                }
                for (j = 0; j < 8; j++) {
                    ItemGetEquipStats(stats[j][k], cur);
                    for (m = 0; m < 6; m++) {
                        diff[j][m] = own[m] - cur[m];
                    }
                    if (r != 0) {
                        ent[cnt].stats[j] = diff[j][r - 1];
                    } else {
                        ent[cnt].stats[j] = 0;
                    }
                }
                ent[cnt].unk19 = r;
                ent[cnt].price = ptyItemTable[x].price;
                cnt++;
            }
        }
    }
    itemWork->unk110E = cnt;
}
