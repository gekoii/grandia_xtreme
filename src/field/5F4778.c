#include "common.h"
#include "party.h"
#include "field/menu.h"
#include "field/n19.h"
#include "protos/field.h"

extern u8 itemEggChartTypeTable[];
extern u8 itemEggChartGrid[7][6];
extern u8 itemEggChartRecipeTable[][6][2];

void ItemEggChartBuild(void) {
    u8 cnt[45];
    s32 i;
    s32 k;
    s32 n;
    ItemJuncEntry *e;
    u8 id;
    u8 r;
    s16 v;
    u8 a;
    u8 b;
    s32 ra;
    s32 rb;
    s32 x;
    s32 xb;
    s32 v2;
    s32 m;
    s32 l;
    s32 idx;

    for (i = 0; i < 7; i++) {
        for (k = 0; k < 6; k++) {
            itemWork->u76F8.junc.grid[i][k] = 0;
            itemWork->u76F8.junc.grid2[i][k] = 0;
        }
    }
    for (i = 0; i < 45; i++) {
        cnt[i] = 0;
    }
    for (i = 0; i < 43; i++) {
        itemWork->u76F8.junc.ents[i].unk0 = 0;
        if (i == 0) {
            continue;
        }
        itemWork->u76F8.junc.ents[i].unk0 = 1;
        e = &itemWork->u76F8.junc.ents[i];
        id = itemEggChartTypeTable[i];
        idx = id + 0x12C;
        e->unk3 = ptyItemTable[idx].icon;
        e->unk8 = ptyItemTable[idx].name;
        e->item = &ptyItemTable[idx];
        r = ItemGetUseClass(1, idx);
        e->unk4 = 0;
        for (k = 0; k < 24; k++) {
            if (ptyPartyData->eggs[k].itemId != 0) {
                if (ptyPartyData->eggs[k].owner == -1 || PtyCharIsJoined(ptyPartyData->eggs[k].owner) >= 0) {
                    xb = itemEggChartCellTable[ptyPartyData->eggs[k].itemId - 0x12C];
                    if (xb == i) {
                        e->unk4++;
                    }
                }
            }
        }
        n = 0;
        for (k = 0; k < 6; k++) {
            v = ptyManaEggTable[id].spells[k];
            if (v != 0) {
                e->s1[n].unk0 = v;
                e->s1[n].unk1 = ptyMoveTable[v].icon;
                e->s1[n].unk4 = ptyMoveTable[v].target;
                e->s1[n].unk5 = r;
                e->s1[n].unk8 = ptyMoveTable[v].name;
                e->s1[n].unk2 = *(u16 *)*(s32 *)&ptyMoveTable[v].levels; /* levels->mpCost, read through an s32: the pointer load's alias set keeps the schedule */
                n++;
            }
        }
        e->unk2 = n;
        n = 0;
        for (k = 0; k < 6; k++) {
            a = itemEggChartRecipeTable[i][k][0];
            b = itemEggChartRecipeTable[i][k][1];
            if (a != 0) {
                ra = ItemEggChartIsKnown(itemEggChartCellTable[a]);
                rb = ItemEggChartIsKnown(itemEggChartCellTable[b]);
                if (ra == 0 || rb == 0) {
                    e->s2[n].unk4 = 0;
                    e->s2[n].unk8 = 0;
                    if (ra == 0) {
                        e->s2[n].unk0 = ptyItemTable[a + 0x12C].icon;
                        e->s2[n].unk4 = ptyItemTable[a + 0x12C].name;
                    }
                    if (rb == 0) {
                        e->s2[n].unk1 = ptyItemTable[b + 0x12C].icon;
                        e->s2[n].unk8 = ptyItemTable[b + 0x12C].name;
                    }
                    n++;
                }
            }
        }
        e->unk1 = n;
    }
    itemWork->u76F8.junc.unk78FA = 0;
    for (i = 0; i < 24; i++) {
        if (ptyPartyData->eggs[i].itemId != 0) {
            if (ptyPartyData->eggs[i].owner == -1 || PtyCharIsJoined(ptyPartyData->eggs[i].owner) >= 0) {
                xb = ptyPartyData->eggs[i].itemId;
                xb = itemEggChartCellTable[xb - 0x12C];
                cnt[xb]++;
                itemWork->u76F8.junc.unk78FA++;
            }
        }
    }
    for (i = 0; i < 7; i++) {
        for (k = 0; k < 6; k++) {
            v2 = itemEggChartGrid[i][k];
            x = cnt[v2] ? (v2 | 0x40) : v2;
            x = ItemEggChartIsKnown(v2) == 0 ? (x | 0x80) : x;
            itemWork->u76F8.junc.grid[k][i] = x;
        }
    }
    itemWork->u76F8.junc.links[4] = 1;
    if (itemWork->u76F8.junc.grid[0][0] & 0x80) {
        itemWork->u76F8.junc.links[0] = 1;
        itemWork->u76F8.junc.links[3] = 1;
        itemWork->u76F8.junc.links[1] = 1;
    }
    if (itemWork->u76F8.junc.grid[5][0] & 0x80) {
        itemWork->u76F8.junc.links[6] = 1;
        itemWork->u76F8.junc.links[3] = 1;
        itemWork->u76F8.junc.links[7] = 1;
    }
    if (itemWork->u76F8.junc.grid[0][6] & 0x80) {
        itemWork->u76F8.junc.links[2] = 1;
        itemWork->u76F8.junc.links[1] = 1;
        itemWork->u76F8.junc.links[5] = 1;
    }
    if (itemWork->u76F8.junc.grid[5][6] & 0x80) {
        itemWork->u76F8.junc.links[8] = 1;
        itemWork->u76F8.junc.links[7] = 1;
        itemWork->u76F8.junc.links[5] = 1;
    }
    m = 0;
    for (l = 1; l < 5; l++) {
        if (itemWork->u76F8.junc.grid[l][m] & 0x80) {
            itemWork->u76F8.junc.links[3] = 1;
        }
    }
    m = 6;
    for (l = 1; l < 5; l++) {
        if (itemWork->u76F8.junc.grid[l][m] & 0x80) {
            itemWork->u76F8.junc.links[5] = 1;
        }
    }
    l = 0;
    for (m = 1; m < 6; m++) {
        if (itemWork->u76F8.junc.grid[l][m] & 0x80) {
            itemWork->u76F8.junc.links[1] = 1;
        }
    }
    l = 5;
    for (m = 1; m < 6; m++) {
        if (itemWork->u76F8.junc.grid[l][m] & 0x80) {
            itemWork->u76F8.junc.links[7] = 1;
        }
    }
    for (m = 1; m < 6; m++) {
        for (l = 1; l < 5; l++) {
            if (itemWork->u76F8.junc.grid[l][m] & 0x80) {
                itemWork->u76F8.junc.links[4] = 1;
            }
        }
    }
    for (m = 0; m < 7; m++) {
        for (l = 0; l < 6; l++) {
            itemWork->u76F8.junc.grid2[l][m] = 0;
        }
    }
    for (m = 1; m < 6; m++) {
        for (l = 1; l < 5; l++) {
            itemWork->u76F8.junc.grid2[l][m] = 1;
        }
    }
    if (itemWork->u76F8.junc.links[0] != 0) {
        itemWork->u76F8.junc.grid2[0][0] = 1;
    }
    if (itemWork->u76F8.junc.links[6] != 0) {
        itemWork->u76F8.junc.grid2[5][0] = 1;
    }
    if (itemWork->u76F8.junc.links[2] != 0) {
        itemWork->u76F8.junc.grid2[0][6] = 1;
    }
    if (itemWork->u76F8.junc.links[8] != 0) {
        itemWork->u76F8.junc.grid2[5][6] = 1;
    }
    if (itemWork->u76F8.junc.links[3] != 0) {
        m = 0;
        for (l = 1; l < 5; l++) {
            itemWork->u76F8.junc.grid2[l][m] = 1;
        }
    }
    if (itemWork->u76F8.junc.links[5] != 0) {
        m = 6;
        for (l = 1; l < 5; l++) {
            itemWork->u76F8.junc.grid2[l][m] = 1;
        }
    }
    if (itemWork->u76F8.junc.links[1] != 0) {
        l = 0;
        for (m = 1; m < 6; m++) {
            itemWork->u76F8.junc.grid2[l][m] = 1;
        }
    }
    if (itemWork->u76F8.junc.links[7] != 0) {
        l = 5;
        for (m = 1; m < 6; m++) {
            itemWork->u76F8.junc.grid2[l][m] = 1;
        }
    }
}
