#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "sys.h"
#include "field/n23.h"
#include "field/n24.h"

/* itemWork records read through views where field/menu.h's types don't fit:
 * the help fields are pointers there but are read as words here (a pointer
 * load schedules differently), and the book list's slot arrays are not
 * arrays in ItemRec. */
typedef struct {
    u8 pad0[0x10];
    s32 help;
    u8 pad14[4];
} ItemBagView; /* ItemBagRec */

typedef struct {
    u8 pad0[0x14];
    s32 help;
} ItemEggView; /* ItemEggListRec */

typedef struct {
    u8 pad0[6];
    s8 owners[3];
    s8 icons[3];
    u8 padC[0x10];
} ItemBookView; /* ItemBookRec */

typedef struct {
    u8 pad0[8];
    s32 help;
} ItemValuableView; /* ItemValuable */

typedef struct {
    u8 pad0[0x1140];
    ItemBagView bag[30];
    ItemEggView eggs[24];
    ItemBookView books[24];
    u8 pad18F0[0x23E0 - 0x18F0];
    ItemValuableView valuables[24];
} ItemListWork;

extern const SprRect ipIconPartyRects[];

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);
FldMenuQuad *FldMenuTaggedNumberBuild(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32, s32);
void VuSinCosDeg(f32 *, f32 *, f32);

s32 FldMenuItemListLayout(u8 id, s32 n, s32 cnt) {
    f32 sn;
    f32 cs;
    FldMenuQuad *h;
    FldMenuQuad *s1;
    FldMenuQuad *s2;
    FldMenuQuad *s3;
    FldMenuQuad *s4;
    FldMenuQuad *s5;
    s32 m;
    s32 idx;
    s32 i;
    s32 k;
    s32 sel;
    s32 w;
    s32 icon;
    s32 xo;
    s32 up;
    s32 dn;
    s32 base;

    h = FldMenuWinGetQuadBuf(id);
    s1 = FldMenuWinGetSprBuf(id, 1);
    FldMenuWinSetLayer(id, 1, &fldMenuTextures[1], s1, 128.0f);
    m = 8;
    s2 = FldMenuWinGetSprBuf(id, 2);
    FldMenuWinSetLayer(id, 2, &fldMenuTextures[0], s2, 128.0f);
    s3 = FldMenuWinGetSprBuf(id, 3);
    FldMenuWinSetLayer(id, 3, &fldMenuTextures[2], s3, 128.0f);
    s4 = FldMenuWinGetSprBuf(id, 4);
    FldMenuWinSetLayer(id, 4, &fldMenuTextures[5], s4, 128.0f);
    s5 = FldMenuWinGetSprBuf(id, 5);
    FldMenuWinSetLayer(id, 5, &fldMenuTextures[4], s5, 128.0f);

    if (fldMenuItem.category == 1) {
        s3->v[0].r = 0x80;
        s3->v[0].g = 0x80;
        s3->v[0].b = 0x80;
        s3->v[0].a = 0x80;
        s3->v[0].x = 0x150;
        s3->v[0].y = -0x10;
        s3->v[1].x = 0x16E;
        s3->v[1].y = 4;
        s3->v[0].u = 0xCE;
        s3->v[0].v = 0x6E;
        s3->v[1].u = 0xEC;
        s3->v[1].v = 0x82;
        s3->type = 1;
        s3->z = 0;
        s3++;
        FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], 0x198, -0x10, 0x12, 0x12, n++, 0xB, 2);
    }
    if (fldMenuItem.category == 3) {
        FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], 0x108, -0x10, 0x10, 0x12, n++, 9, 2);
        FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], 0x1B0, -0x10, 0x12, 0x12, n++, 0xB, 2);
    }
    if (fldMenuItem.category == 0) {
        FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], 0x15E, -0x10, 0x10, 0x12, 0x18, 7, 2);
        FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], 0x1A4, -0x10, 0x12, 0x12, 0x1A, 0xC, 2);
    }
    if (fldMenuItem.category == 2) {
        FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], 0x124, -0x10, 0x10, 0x12, n++, 0xA, 2);
        FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], 0x1B0, -0x10, 0x12, 0x12, n++, 0xB, 2);
    }

    idx = fldMenuItem.top;
    icon = 0;
    if (idx != 0) {
        up = 1;
    } else {
        up = 0;
    }
    if (cnt >= 8) {
        if (idx + 7 < cnt) {
            dn = 1;
        } else {
            dn = 0;
        }
    } else {
        dn = 0;
    }
    FldMenuWinSetScrollArrows(id, up, dn);
    if (cnt >= 8) {
        cnt = 7;
    }
    xo = 0x14;
    base = 0x100;
    fldMenuItem.helpId = 0;
    fldMenuItem.helpType = 0;
    for (i = 0; i < cnt; idx++, i++) {
        if (i == fldMenuItem.cursor) {
            sel = 8;
            fldMenuItem.marks[idx] -= 8;
            if (fldMenuItem.marks[idx] < (cnt - i) * 6) {
                fldMenuItem.marks[idx] = (cnt - i) * 6;
            }
            w = 0x16;
        } else {
            sel = 0;
            fldMenuItem.marks[idx] += 8;
            if (fldMenuItem.marks[idx] >= (cnt - i) * 6 + 0x10) {
                fldMenuItem.marks[idx] = (cnt - i) * 6 + 0x10;
            }
            w = 0x14;
        }
        switch (fldMenuItem.category) {
        case 0:
            if (sel == 8) {
                if (itemWork->bag[idx].unk8 != -1) {
                    fldMenuItem.helpType = itemWork->bag[idx].unk8;
                    fldMenuItem.helpId = ((ItemListWork *)itemWork)->bag[idx].help;
                }
            }
            if (itemWork->bag[idx].unk6 != 0) {
                sel = 10;
            }
            FldMenuWinTextAdd(id, fldMenuItem.page, itemWork->bag[idx].unkC, fldMenuItem.marks[idx] + 0x34, i * 0x18 + 8, w, w, n++, -1, sel);
            if ((s8)itemWork->bag[idx].unk5 != 0) {
                FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[9], fldMenuItem.marks[idx] + 0x134, i * 0x18 + 8, w, w, m++,
                              (s8)itemWork->bag[idx].unk5, 0);
            }
            if (itemWork->bag[idx].unk6 != 1) {
                s2 = FldMenuTaggedNumberBuild(s2, itemWork->bag[idx].count, 0, fldMenuItem.marks[idx] + 0x17C, i * 0x18 + 8, 1.0f, 1.0f, 0, 0);
            }
            icon = (s8)itemWork->bag[idx].unk2;
            h = FldMenuBarBuild(h, fldMenuItem.marks[idx] + 0xC, i * 0x18 + 0x16, 0x1A4, 0xC, itemWork->bag[idx].unk6);
            break;
        case 1: {
            s32 y;
            if (sel == 8) {
                fldMenuItem.helpType = 3;
                fldMenuItem.helpId = ((ItemListWork *)itemWork)->eggs[idx].help;
            }
            FldMenuWinTextAdd(id, fldMenuItem.page, itemWork->eggs[idx].name, fldMenuItem.marks[idx] + 0x34, i * 0x18 + 8, w, w, n++, -1, sel);
            for (k = 0; k < itemWork->eggs[idx].rarity; k++) {
                s32 x;
                s32 y;
                s32 yb;
                VuSinCosDeg(&sn, &cs, (f32)k * 110.0f + (f32)fldMenuItem.ang);
                s5->type = 1;
                if (itemWork->eggs[idx].rarity == 1) {
                    s5->v[0].a = 0;
                    s5->v[0].r = 0x80;
                    s5->v[0].g = 0x80;
                    s5->v[0].b = 0x80;
                } else {
                    s5->v[0].r = 0xA0;
                    s5->v[0].g = 0x80;
                    s5->v[0].b = 0x80;
                    s5->v[0].a = 0;
                }
                s5->v[0].x = fldMenuItem.marks[idx] + xo + (x = cs * 7.0f);
                y = sn * 5.0f;
                yb = i * 0x18 + 8;
                s5->v[0].y = yb + y;
                s5->v[1].x = fldMenuItem.marks[idx] + x + 0x1E;
                s5->v[1].y = y + i * 0x18 + 0x12;
                s5->v[0].u = sprStarIconRects[0].u0;
                s5->v[0].v = sprStarIconRects[0].v0;
                s5->v[1].u = sprStarIconRects[0].u1;
                s5->v[1].v = sprStarIconRects[0].v1;
                s5->z = 0;
                s5++;
            }
            s2 = FldMenuHpFractionBuild(s2, itemWork->eggs[idx].mp, itemWork->eggs[idx].maxMp, fldMenuItem.marks[idx] + 0x104, (y = i * 0x18 + 8) + 4);
            if (itemWork->eggs[idx].owner >= 0) {
                s32 hh;
                s4->type = 1;
                s4->v[0].r = 0x80;
                s4->v[0].g = 0x80;
                s4->v[0].b = 0x80;
                s4->v[0].a = 0x80;
                s4->v[0].x = fldMenuItem.marks[idx] + 0x184;
                s4->v[0].y = y + 4;
                hh = w + 4;
                s4->v[1].x = fldMenuItem.marks[idx] + w + 0x184;
                s4->v[1].y = y + hh;
                s4->z = 0;
                s4->v[0].u = ipIconPartyRects[itemWork->eggs[idx].owner].u0;
                s4->v[0].v = ipIconPartyRects[itemWork->eggs[idx].owner].v0;
                s4->v[1].u = ipIconPartyRects[itemWork->eggs[idx].owner].u1;
                s4->v[1].v = ipIconPartyRects[itemWork->eggs[idx].owner].v1;
                s4++;
            }
            h = FldMenuBarBuild(h, fldMenuItem.marks[idx] + 0xC, i * 0x18 + 0x16, 0x1A4, 0xC, itemWork->eggs[idx].useClass);
            icon = (s8)itemWork->eggs[idx].icon;
            break;
        }
        case 3: {
            s32 y;
            if (sel == 8) {
                fldMenuItem.helpType = 2;
                fldMenuItem.helpId = itemWork->books2[idx].help;
            }
            FldMenuWinTextAdd(id, fldMenuItem.page, itemWork->books2[idx].name, fldMenuItem.marks[idx] + 0x34, i * 0x18 + 8, w, w, n++, -1, sel);
            h = FldMenuBarBuild(h, fldMenuItem.marks[idx] + 0xC, i * 0x18 + 0x16, 0x1A4, 0xC, itemWork->books2[idx].type + 0xB);
            ((void (*)(u8, u8, s32, s32, s32, s32, s32, s32, s32))FldMenuWinTextAddIndexed)(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[10], fldMenuItem.marks[idx] + 0xFE, i * 0x18 + 8, w, w, itemWork->books2[idx].type, 3);
            if (fldMenuItem.skillGauge > 0x80) {
                for (k = 0; k < itemWork->books2[idx].n; k++) {
                    s5->v[0].r = 0x80;
                    s5->v[0].g = 0x80;
                    s5->v[0].b = 0x80;
                    s5->v[0].a = 0;
                    s5->type = 1;
                    y = i * 0x18 + 8;
                    s5->v[0].x = fldMenuItem.marks[idx] + k * 16 + 0x11C;
                    s5->v[0].y = y;
                    s5->v[1].x = fldMenuItem.pipLen[k] + (fldMenuItem.marks[idx] + k * 16) + xo + 0x108;
                    s5->v[1].y = fldMenuItem.pipLen[k] + y;
                    s5->v[0].u = sprStarIconRects[0].u0;
                    s5->v[0].v = sprStarIconRects[0].v0;
                    s5->v[1].u = sprStarIconRects[0].u1;
                    s5->v[1].v = sprStarIconRects[0].v1;
                    s5->z = 0;
                    s5++;
                }
                if (itemWork->books2[idx].n == itemWork->books2[idx].max) {
                    s5->v[0].r = 0x80;
                    s5->v[0].g = 0x80;
                    s5->type = 1;
                    s5->v[0].b = 0x80;
                    s5->v[0].a = 0;
                    s5->v[0].x = fldMenuItem.marks[idx] + 0x120;
                    s5->v[0].y = i * 0x18 + 4;
                    s5->v[1].x = (u16)sprStarIconRects[1].w + fldMenuItem.marks[idx] + 0x120;
                    s5->v[1].y = (u16)sprStarIconRects[1].h + i * 0x18 + 4;
                    s5->v[0].u = sprStarIconRects[1].u0;
                    s5->v[0].v = sprStarIconRects[1].v0;
                    s5->v[1].u = sprStarIconRects[1].u1;
                    s5->v[1].v = sprStarIconRects[1].v1;
                    s5->z = 0;
                    s5++;
                }
            } else {
                s2 = FldMenuFractionBuild(s2, itemWork->books2[idx].unkC, itemWork->books2[idx].unkE, fldMenuItem.marks[idx] + 0x11C, i * 0x18 + 0xA);
            }
            icon = itemWork->books2[idx].icon;
            if (itemWork->books2[idx].unk3 >= 0) {
                s4->v[0].r = 0x80;
                s4->v[0].g = 0x80;
                s4->v[0].b = 0x80;
                s4->v[0].a = 0x80;
                s4->type = 1;
                s4->v[0].x = fldMenuItem.marks[idx] + 0x18E;
                s4->v[0].y = i * 0x18 + 0xC;
                s4->v[1].x = fldMenuItem.marks[idx] + 0x1A6;
                s4->v[1].y = i * 0x18 + 0x24;
                s4->z = 0;
                s4->v[0].u = ipIconPartyRects[itemWork->books2[idx].unk3].u0;
                s4->v[0].v = ipIconPartyRects[itemWork->books2[idx].unk3].v0;
                s4->v[1].u = ipIconPartyRects[itemWork->books2[idx].unk3].u1;
                s4->v[1].v = ipIconPartyRects[itemWork->books2[idx].unk3].v1;
                s4++;
            }
            break;
        }
        case 2: {

            if (sel == 8) {
                fldMenuItem.helpType = 3;
                fldMenuItem.helpId = itemWork->books[idx].help;
            }
            FldMenuWinTextAdd(id, fldMenuItem.page, itemWork->books[idx].name, fldMenuItem.marks[idx] + 0x34, i * 0x18 + 8, w, w, n++, -1, sel);
            h = FldMenuBarBuild(h, fldMenuItem.marks[idx] + 0xC, i * 0x18 + 0x16, 0x1A4, 0xC, itemWork->books[idx].type + 0xB);
            ((void (*)(u8, u8, s32, s32, s32, s32, s32, s32, s32))FldMenuWinTextAddIndexed)(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[10], fldMenuItem.marks[idx] + 0x116, i * 0x18 + 8, w, w, itemWork->books[idx].type, 3);
            for (k = 0; k < itemWork->books[idx].n; k++) {
                if (((ItemListWork *)itemWork)->books[idx].owners[k] == -1) {
                    s32 x;
                    s32 y;
                    s1->v[0].r = 0x80;
                    s1->v[0].g = 0x80;
                    s1->type = 1;
                    s1->v[0].b = 0x80;
                    s1->v[0].a = 0;
                    x = (k + 6) * 0x18 + 0x98;
                    y = i * 0x18 + 8;
                    s1->v[0].x = fldMenuItem.marks[idx] + xo + x;
                    s1->v[0].y = y;
                    s1->v[1].x = menuItemIcons[78].w + xo + x + fldMenuItem.marks[idx];
                    s1->v[1].y = menuItemIcons[78].h + y;
                    s1->v[0].u = menuItemIcons[78].u0;
                    s1->v[0].v = menuItemIcons[78].v0;
                    s1->v[1].u = menuItemIcons[78].u1;
                    s1->v[1].v = menuItemIcons[78].v1;
                    s1->z = 0;
                    s1++;
                } else {
                    s32 x;
                    s32 y;
                    icon = ((ItemListWork *)itemWork)->books[idx].icons[k];
                    s5->v[0].r = 0x80;
                    s5->v[0].g = 0x80;
                    s5->type = 1;
                    s5->v[0].b = 0x80;
                    s5->v[0].a = 0;
                    x = (k + 6) * 0x18 + 0x98;
                    s5->v[0].x = fldMenuItem.marks[idx] + xo + x;
                    y = i * 0x18 + 8;
                    s5->v[0].y = y;
                    s5->v[1].x = (u16)menuSkillTypeIcons[icon].w + xo + x + fldMenuItem.marks[idx];
                    s5->v[1].y = (u16)menuSkillTypeIcons[icon].h + y;
                    s5->v[0].u = menuSkillTypeIcons[icon].u0;
                    s5->v[0].v = menuSkillTypeIcons[icon].v0;
                    s5->v[1].u = menuSkillTypeIcons[icon].u1;
                    s5->v[1].v = menuSkillTypeIcons[icon].v1;
                    s5->z = 0;
                    s5++;
                }
            }
            icon = itemWork->books[idx].icon;
            if (itemWork->books[idx].unk3 >= 0) {
                s4->type = 1;
                s4->v[0].r = 0x80;
                s4->v[0].g = 0x80;
                s4->v[0].b = 0x80;
                s4->v[0].a = 0x80;
                s4->v[0].x = fldMenuItem.marks[idx] + 0x18E;
                s4->v[0].y = i * 0x18 + 0xC;
                s4->v[1].x = fldMenuItem.marks[idx] + 0x1A6;
                s4->v[1].y = i * 0x18 + 0x24;
                s4->z = 0;
                s4->v[0].u = ipIconPartyRects[itemWork->books[idx].unk3].u0;
                s4->v[0].v = ipIconPartyRects[itemWork->books[idx].unk3].v0;
                s4->v[1].u = ipIconPartyRects[itemWork->books[idx].unk3].u1;
                s4->v[1].v = ipIconPartyRects[itemWork->books[idx].unk3].v1;
                s4++;
            }
            break;
        }
        case 4:
            if (sel == 8) {
                fldMenuItem.helpType = 3;
                fldMenuItem.helpId = ((ItemListWork *)itemWork)->valuables[idx].help;
            }
            FldMenuWinTextAdd(id, fldMenuItem.page, itemWork->valuables[idx].unk4, fldMenuItem.marks[idx] + 0x34, i * 0x18 + 8, w, w, n++, -1, sel);
            icon = (s8)itemWork->valuables[idx].unk2;
            h = FldMenuBarBuild(h, fldMenuItem.marks[idx] + 0xC, i * 0x18 + 0x16, 0x1A4, 0xC, 0);
            break;
        case 5: {

            icon = idx + 6;
            for (k = 0; k < itemWork->rows[idx].count; k++) {
                s1->type = 1;
                if (itemWork->rows[idx].a[k] == 0) {
                    s1->v[0].a = 0;
                    s1->v[0].r = 0x20;
                    s1->v[0].g = 0x20;
                    s1->v[0].b = 0x20;
                } else {
                    s1->v[0].a = 0;
                    s1->v[0].r = 0x80;
                    s1->v[0].g = 0x80;
                    s1->v[0].b = 0x80;
                }
                if (itemWork->rows[idx].b[k] == 0) {
                    s32 y;
                    s1->v[0].x = fldMenuItem.marks[idx] + k * 0x14 + base + xo;
                    y = i * 0x18 + 8;
                    s1->v[0].y = y;
                    s1->v[1].x = (s16)(menuItemIcons[icon].w + fldMenuItem.marks[idx] + base + xo) + k * 0x14;
                    s1->z = 0;
                    s1->v[1].y = menuItemIcons[icon].h + y;
                } else {
                    s32 y;
                    y = i * 0x18 + 8;
                    s1->v[0].x = (k * 0x14 + xo) + (fldMenuItem.marks[idx] + base) - fldMenuItem.shrink;
                    s1->v[0].y = y;
                    s1->v[1].x = (k * 0x14 + xo) + (fldMenuItem.marks[idx] + base) - fldMenuItem.shrink + (menuItemIcons[icon].w - fldMenuItem.shrink);
                    s1->v[1].y = y + (menuItemIcons[icon].h - fldMenuItem.shrink);
                    s1->z = 0;
                }
                s1->v[0].u = menuItemIcons[icon].u0;
                s1->v[0].v = menuItemIcons[icon].v0;
                s1->v[1].u = menuItemIcons[icon].u1;
                s1->v[1].v = menuItemIcons[icon].v1;
                s1++;
            }
            if (itemWork->rows[idx].flag == 0) {
                FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[14], fldMenuItem.marks[idx] + 0x24, i * 0x18 + 8, w, w, n++, 5, 0);
            } else {
                FldMenuWinTextAdd(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[14], fldMenuItem.marks[idx] + 0x24, i * 0x18 + 8, w, w, n++, idx, sel + 0x28);
            }
            h = FldMenuBarBuild(h, fldMenuItem.marks[idx] + 0xC, i * 0x18 + 0x16, 0x1A4, 0xC, 0xA);
            break;
        }
        }
        if (fldMenuItem.category == 3) {

            s32 y;
            s5->v[0].r = 0x80;
            s5->v[0].g = 0x80;
            s5->type = 1;
            s5->v[0].b = 0x80;
            s5->v[0].a = 0;
            y = i * 0x18 + 8;
            s5->v[0].x = fldMenuItem.marks[idx] + xo;
            s5->v[0].y = y;
            s5->v[1].x = fldMenuItem.marks[idx] + xo + (u16)menuSkillTypeIcons[icon].w;
            s5->v[1].y = (u16)menuSkillTypeIcons[icon].h + y;
            s5->v[0].u = menuSkillTypeIcons[icon].u0;
            s5->v[0].v = menuSkillTypeIcons[icon].v0;
            s5->v[1].u = menuSkillTypeIcons[icon].u1;
            s5->v[1].v = menuSkillTypeIcons[icon].v1;
            s5->z = 0;
            s5++;
        } else if (fldMenuItem.category != 5) {

            s32 y;
            s1->v[0].r = 0x80;
            s1->v[0].g = 0x80;
            s1->type = 1;
            s1->v[0].b = 0x80;
            s1->v[0].a = 0;
            y = i * 0x18 + 8;
            s1->v[0].x = fldMenuItem.marks[idx] + xo;
            s1->v[0].y = y;
            s1->v[1].x = fldMenuItem.marks[idx] + xo + menuItemIcons[icon].w;
            s1->v[1].y = menuItemIcons[icon].h + y;
            s1->v[0].u = menuItemIcons[icon].u0;
            s1->v[0].v = menuItemIcons[icon].v0;
            s1->v[1].u = menuItemIcons[icon].u1;
            s1->v[1].v = menuItemIcons[icon].v1;
            s1->z = 0;
            s1++;
        }
    }
    h->type = 0;
    s1->type = 0;
    s2->type = 0;
    s3->type = 0;
    s4->type = 0;
    s5->type = 0;
    return n;
}
