#include "common.h"
#include "save.h"
#include "field/menu.h"

typedef struct {
    u8 pad0[0x32];
    s16 a[6];
    u8 pad3E[0x84 - 0x3E];
} Rec;

typedef struct {
    u8 pad0[0xF];
    s8 f;
} Info;

typedef struct {
    s8 type;
    u8 unk1;
    u8 pad2[0xA];
    s32 unkC;
    u8 pad10[8];
    Info *info;
    u8 pad1C[4];
} ItemDef;

/* pty* tables through this file's own views (casts at each use): save.h now
 * brings in party.h's declarations, and the views keep the match. */
extern u8 itemEquipSlotOrder[];

void PtyEquipListBuild(s16 arg0, ItemCharPage *chars) {
    s16 *src;
    s32 i;
    s16 id;
    u8 unk1;
    s32 unkC;
    ItemDef *d;
    s32 k;
    ItemDef *tbl;
    s32 n;
    s32 f;
    ItemDef *p;
    ItemCharPage *w;

    n = sysSaveData.party[arg0] - 1;
    src = ((Rec *)ptyCharData)[n].a;
    tbl = ((ItemDef *)ptyItemTable);
    for (i = 0; i < 6; i++) {
        unk1 = 0;
        id = src[i];
        unkC = 0;
        d = NULL;
        w = (ItemCharPage *)((u8 *)itemWork + (arg0 * 0x43C + i * 12)); /* &itemWork->chars[arg0], shifted by i slots: the original's address arithmetic */
        w->equip[0].id = id;
        if (id != 0) {
            p = (ItemDef *)((id << 5) + (u32)tbl);
            f = ((ItemDef *)ptyItemTable)[id].info->f;
            w->equip[0].unk3 = f;
            unk1 = p->unk1;
            unkC = p->unkC;
            d = p;
        }
        k = itemEquipSlotOrder[i];
        chars[arg0].equip[k].unk2 = unk1;
        chars[arg0].equip[k].name = unkC;
        chars[arg0].equip[k].desc = (s32)d;
    }
}
