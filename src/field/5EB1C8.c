#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/menu.h"

typedef struct {
    u8 pad0[0x32];
    s16 a[6];
    s16 b[6];
    u8 pad4A[0x68 - 0x4A];
    u8 x68;
    u8 x69;
    u8 pad6A[0x84 - 0x6A];
} Rec;

typedef struct {
    u8 pad0[0x60];
    s8 n;
    u8 pad61[0x7C - 0x61];
} Party;

typedef struct {
    u8 pad0[0xC];
    u16 xC;
    u8 padE[2];
    s32 x10;
    s32 x14;
    s32 x18;
    s32 x1C;
    s32 x20;
    s32 x24;
    s32 x28;
    s32 x2C;
    s32 x30[6];
    u8 pad48[8];
    s32 x50[6];
    u8 pad68[0x318 - 0x68];
} Unit;

typedef struct {
    s8 type;
    u8 unk1;
    u8 pad2[0x1E];
} ItemDef;

typedef struct {
    s32 x0;
    u8 pad4[0x10FA - 4];
    struct {
        s16 id;
        s8 b;
        s8 c;
    } slots[24];
    u8 pad115A[0x147A - 0x115A];
    struct {
        s16 id;
        u8 pad2[0x1A];
    } e[24];
} World;


/* pty* tables through this file's own views (casts at each use): save.h now
 * brings in party.h's declarations, and the views keep the match. */

void ItemPartyStatusBuild(s32 arg0) {
    s16 a;
    s16 b;
    s32 i;
    s32 j;
    ItemWork *w;
    ItemCharPage *e;
    Rec *r;
    Unit *u;
    s16 id;
    s16 t;
    s32 n;

    w = itemWork;
    w->gold = ((World *)ptyPartyData)->x0;
    for (i = 0; i < 4; i++) {
        e = &w->chars[i];
        e->chr = sysSaveData.party[i];
        if (e->chr != 0) {
            u = &((Unit *)ptyPartyUnits)[i];
            r = &((Rec *)ptyCharData)[e->chr - 1];
            e->tactics[0] = r->x68;
            e->tactics[1] = r->x69;
            e->hp = u->x1C;
            e->level = u->xC;
            e->hpMax = u->x18;
            e->mp = u->x24;
            e->mpMax = u->x20;
            e->sp = u->x2C;
            e->spMax = u->x28;
            e->unk8 = u->x10;
            e->unkC = u->x14;
            e->stats[0] = u->x30[0];
            e->stats[1] = u->x30[1];
            e->stats[2] = u->x30[2];
            e->stats[3] = u->x30[3];
            e->stats[4] = u->x30[4];
            e->stats[5] = u->x30[5];
            e->stats2[0] = u->x50[0];
            e->stats2[1] = u->x50[1];
            e->stats2[2] = u->x50[2];
            e->stats2[3] = u->x50[3];
            e->stats2[4] = u->x50[4];
            e->stats2[5] = u->x50[5];
            PtyMemberGetStatusIcons(e->chr - 1, &a, &b);
            e->statusIcons[0] = a;
            e->statusIcons[1] = b;
            PtyEquipListBuild(i, w->chars);
            e->unk38[0] = 0;
            e->unk38[1] = 0;
            e->unk38[2] = 0;
            e->unk38[3] = 0;
            e->unk38[4] = 0;
            e->unk38[5] = 0;
            ItemStatusResistBuild(i);
            ItemStatusParamsBuild(i);
            ItemSpecialMoveListBuild(i);
            ItemComboListBuild(i);
        }
    }
    for (i = 0; i < 8; i++) {
        n = ((Party *)ptyCharDefTable)[i].n;
        for (j = 0; j < 6; j++) {
            id = ((Rec *)ptyCharData)[i].b[j];
            if (id == -1) {
                itemWork->slotIcons[i][j] = 'N';
            } else {
                if (j < n) {
                    t = ((World *)ptyPartyData)->e[id].id;
                    if (t == 0) {
                        itemWork->slotIcons[i][j] = 'N';
                    } else {
                        itemWork->slotIcons[i][j] = ((ItemDef *)ptyItemTable)[t].unk1;
                    }
                } else {
                    t = ((World *)ptyPartyData)->slots[id].id;
                    if (t == 0) {
                        itemWork->slotIcons[i][j] = 'N';
                    } else {
                        itemWork->slotIcons[i][j] = ((ItemDef *)ptyItemTable)[t].unk1;
                    }
                }
            }
        }
    }
}
