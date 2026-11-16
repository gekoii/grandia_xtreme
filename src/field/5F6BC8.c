#include "common.h"
#include "protos/field.h"
#include "save.h"

typedef struct {
    s16 id;
    u8 pad2;
    s8 who;
    s16 max;
    s16 lv;
    u8 pad8[0x14];
} Slot;

typedef struct {
    u8 pad0[0x22];
    struct {
        s16 id;
        u8 pad2[2];
    } e[1302];
    Slot slot[24];
} World;

typedef struct {
    s8 kind;
    u8 pad1;
    s16 base;
    u8 pad4[0xC];
    s32 flags;
    u8 pad14[4];
    u8 stat;
} Act;

typedef struct {
    u8 pad0[2];
    s8 target;
    u8 pad3;
    u16 x4;
    u8 pad6[0xA];
    Act *act;
} Info;

typedef struct {
    s8 kind;
    u8 pad1[0x1B];
    Info *info;
} Item;

typedef struct {
    u8 pad0[4];
    s16 id;
    u8 pad6[0x12];
    s32 hpmax;
    s32 hp;
    u8 pad20[8];
    s32 mpmax;
    s32 mp;
    u8 pad30[0x18];
    s32 status;
    u8 pad4C[0x318 - 0x4C];
} Chr;

typedef struct {
    u8 pad0[4];
    s32 x4;
    u8 pad8[4];
    s16 xC;
    u8 padE[2];
    s16 x10;
    s16 x12;
    s16 x14;
    s16 x16;
    s16 x18;
    s16 x1A;
    u8 pad1C[0x84 - 0x1C];
} Rec;


/* pty* tables through this file's own views (casts at each use): save.h now
 * brings in party.h's declarations, and the views keep the match. */
extern u8 itemFieldUseKindTable[];


s32 ItemUseInField(s16 arg0, s16 arg1) {
    s32 e;
    Item *p;
    Info *info;
    Act *q;
    u16 x4;
    s8 t;
    s8 tg;
    s32 start;
    s32 end;
    s32 i;
    s32 j;
    s32 hit;
    s32 heal;
    s32 seed;
    s32 n;
    s32 c;
    Chr *ch;

    e = ((World *)ptyPartyData)->e[arg0].id;
    if (e == 0) {
        return -1;
    }
    p = (Item *)(((s16)e << 5) + (u32)((Item *)ptyItemTable));
    if (p->kind == 0xD) {
        return -1;
    }
    info = p->info;
    q = info->act;
    t = q->kind;
    tg = info->target;
    x4 = info->x4;
    if (t >= 8) {
        return -1;
    }
    if (p->kind != 4 && itemFieldUseKindTable[t] == 0) {
        return -1;
    }
    if (!(q->flags & 0x8000) && (((Chr *)ptyPartyUnits)[arg1].status & 0x8000)) {
        return -1;
    }
    start = arg1;
    end = start + 1;
    if (tg != 1) {
        start = 0;
        end = 4;
    }
    seed = 0;
    heal = 0;
    hit = 0;
    if (q->flags & 0x9810) {
        for (i = start; i < end; i++) {
            ch = &((Chr *)ptyPartyUnits)[i];
            if (sysSaveData.party[i] != 0) {
                if (q->flags & 0x8000) {
                    if (ch->status & 0x8000) {
                        hit |= 0x8000;
                    }
                }
                if (!(ch->status & 0x8000)) {
                    if (q->flags & 0x800) {
                        if (ch->status & 0x800) {
                            hit |= 0x800;
                        }
                    }
                    if (q->flags & 0x10) {
                        if (ch->status & 0x10) {
                            hit |= 0x10;
                        }
                    }
                    if (q->flags & 0x1000) {
                        if (ch->status & 0x1000) {
                            hit |= 0x1000;
                        }
                    }
                }
            }
        }
    }
    for (i = start; i < end; i++) {
        ch = &((Chr *)ptyPartyUnits)[i];
        if (ch->id != 0 && !(ch->status & 0x8000)) {
            if (t == 1) {
                if (ch->hp != ch->hpmax) {
                    heal |= 1;
                }
            }
            if (t == 2) {
                for (j = 0; j < 24; j++) {
                    if (((World *)ptyPartyData)->slot[j].id != 0 && ((World *)ptyPartyData)->slot[j].who == ch->id - 0x1001) {
                        if (((World *)ptyPartyData)->slot[j].lv < ((World *)ptyPartyData)->slot[j].max) {
                            heal |= 2;
                        }
                    }
                }
            }
            if (t == 3) {
                if (ch->mp != ch->mpmax) {
                    heal |= 4;
                }
            }
        }
    }
    if (((Item *)((e << 5) + (u32)((Item *)ptyItemTable)))->kind == 4) {
        if (((Chr *)ptyPartyUnits)[arg1].id != 0) {
            if (((Chr *)ptyPartyUnits)[arg1].status & 0x8000) {
                seed = 0;
            } else {
                seed = e - 0xA7;
            }
        }
    }
    if ((hit | heal | seed) == 0) {
        return -1;
    }
    if (hit != 0) {
        for (i = start; i < end; i++) {
            ch = &((Chr *)ptyPartyUnits)[i];
            if (sysSaveData.party[i] != 0) {
                if (q->flags & 0x8000) {
                    ch->status &= 0xFFFF7FFF;
                    if (x4 == 0x46) {
                        ch->hp = ch->hpmax / 2;
                    }
                    if (x4 == 0x47) {
                        ch->hp = ch->hpmax;
                    }
                }
                if (!(ch->status & 0x8000)) {
                    if (q->flags & 0x800) {
                        ch->status &= 0xFFFFF7FF;
                    }
                    if (q->flags & 0x10) {
                        ch->status &= 0xFFFFFFEF;
                    }
                    if (q->flags & 0x1000) {
                        ch->status &= 0xFFFFEFFF;
                    }
                }
            }
        }
    }
    if (heal != 0) {
        n = q->base;
        switch (t) {
        case 1:
            for (i = start; i < end; i++) {
                ch = &((Chr *)ptyPartyUnits)[i];
                if (ch->id != 0 && !(ch->status & 0x8000)) {
                    ch->hp += n;
                    if (ch->hpmax < ch->hp) {
                        ch->hp = ch->hpmax;
                    }
                }
            }
            break;
        case 2:
            for (i = start; i < end; i++) {
                ch = &((Chr *)ptyPartyUnits)[i];
                if (ch->id != 0 && !(ch->status & 0x8000)) {
                    for (j = 0; j < 24; j++) {
                        if (((World *)ptyPartyData)->slot[j].id != 0 && ((World *)ptyPartyData)->slot[j].who == ch->id - 0x1001) {
                            ((World *)ptyPartyData)->slot[j].lv += n;
                            if (((World *)ptyPartyData)->slot[j].max < ((World *)ptyPartyData)->slot[j].lv) {
                                ((World *)ptyPartyData)->slot[j].lv = ((World *)ptyPartyData)->slot[j].max;
                            }
                        }
                    }
                }
            }
            break;
        case 3:
            for (i = start; i < end; i++) {
                ch = &((Chr *)ptyPartyUnits)[i];
                if (ch->id != 0 && !(ch->status & 0x8000)) {
                    ch->mp += n;
                    if (ch->mpmax < ch->mp) {
                        ch->mp = ch->mpmax;
                    }
                }
            }
            break;
        }
    }
    if (seed != 0) {
        u8 st = q->stat;
        n = q->base;
        c = ((Chr *)ptyPartyUnits)[start].id - 0x1001;
        switch (st) {
        case 0x3D:
            ((Rec *)ptyCharData)[c].x4 += n;
            break;
        case 0x3E:
            ((Rec *)ptyCharData)[c].xC += n;
            break;
        case 0x3F:
            ((Rec *)ptyCharData)[c].x10 += n;
            break;
        case 0x40:
            ((Rec *)ptyCharData)[c].x12 += n;
            break;
        case 0x41:
            ((Rec *)ptyCharData)[c].x14 += n;
            break;
        case 0x42:
            ((Rec *)ptyCharData)[c].x16 += n;
            break;
        case 0x43:
            ((Rec *)ptyCharData)[c].x18 += n;
            break;
        case 0x44:
            ((Rec *)ptyCharData)[c].x1A += n;
            break;
        case 0x45:
            ((Rec *)ptyCharData)[c].x10 += n;
            ((Rec *)ptyCharData)[c].x12 += n;
            ((Rec *)ptyCharData)[c].x14 += n;
            ((Rec *)ptyCharData)[c].x16 += n;
            ((Rec *)ptyCharData)[c].x18 += n;
            ((Rec *)ptyCharData)[c].x1A += n;
            break;
        }
        if (((Rec *)ptyCharData)[c].x4 > 99998) {
            ((Rec *)ptyCharData)[c].x4 = 99999;
        }
        if (((Rec *)ptyCharData)[c].xC >= 999) {
            ((Rec *)ptyCharData)[c].xC = 999;
        }
        if (((Rec *)ptyCharData)[c].x10 >= 9999) {
            ((Rec *)ptyCharData)[c].x10 = 9999;
        }
        if (((Rec *)ptyCharData)[c].x12 >= 9999) {
            ((Rec *)ptyCharData)[c].x12 = 9999;
        }
        if (((Rec *)ptyCharData)[c].x14 >= 9999) {
            ((Rec *)ptyCharData)[c].x14 = 9999;
        }
        if (((Rec *)ptyCharData)[c].x16 >= 9999) {
            ((Rec *)ptyCharData)[c].x16 = 9999;
        }
        if (((Rec *)ptyCharData)[c].x18 >= 9999) {
            ((Rec *)ptyCharData)[c].x18 = 9999;
        }
        if (((Rec *)ptyCharData)[c].x1A >= 9999) {
            ((Rec *)ptyCharData)[c].x1A = 9999;
        }
    }
    ItemBagRemove(arg0, 1);
    for (i = start; i < end; i++) {
        if (sysSaveData.party[i] != 0) {
            ch = &((Chr *)ptyPartyUnits)[i];
            PtyUnitCalcParams(ch, 0, 0);
            PtyCharStoreUnitStats(ch);
        }
    }
    ItemBagListBuild(0);
    ItemPartyStatusBuild(0);
    return 0;
}
