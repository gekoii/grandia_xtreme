#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "field/menu.h"
#include "field/n24.h"
#include "field/n23.h"

typedef struct {
    s32 flag;
} Box;


FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);
void FldMenuJunctionRecipeCellBuild(FldMenuRecipeCell *);

s32 FldMenuJunctionRecipeUpdate(void) {
    s32 nA;
    s32 nB;
    s32 m;
    s32 k;
    s32 n;
    FldMenuQuad *t;
    Box *h;
    s32 idx;
    u32 minA;
    u32 minB;
    s32 i;
    s32 j;
    s32 l;
    s32 icon;
    s32 name;
    s32 x4;
    s32 val;
    s32 dy;
    u8 sel;
    FldMenuQuad *s;
    FldMenuQuad *s3;
    Box *h2;
    s32 cnt;
    s32 zz;
    s32 ci;
    s32 nm0;
    s32 ic0;

    nB = 5;
    minA = 0;
    minB = 1;
    nA = 6;
    if (!itemWork->u76F8.junc.links[3]) {
        minA = 1;
    }
    if (itemWork->u76F8.junc.links[5]) {
        nA = 7;
    }
    if (itemWork->u76F8.junc.links[1]) {
        minB = 0;
    }
    if (itemWork->u76F8.junc.links[7]) {
        nB = 6;
    }
    fldMenuJunctionRecipe.cell.spr = FldMenuWinGetSprBuf(8, 1);
    fldMenuJunctionRecipe.cell.spr2 = FldMenuWinGetSprBuf(8, 2);
    cnt = 0;
    k = 0;
    n = 0;
    FldMenuWinSetLayer(8, 1, FldOverlayImgGetTex(2), fldMenuJunctionRecipe.cell.spr, 128.0f);
    FldMenuWinSetLayer(8, 2, FldOverlayImgGetTex(3), fldMenuJunctionRecipe.cell.spr2, 128.0f);
    if (fldMenuJunctionRecipe.col <= minB) {
        fldMenuJunctionRecipe.col = minB;
    }
    if (fldMenuJunctionRecipe.row <= minA) {
        fldMenuJunctionRecipe.row = minA;
    }
    if (!fldMenuJunctionRecipe.open) {
        FldMenuCursorSetAt(8, (f32)fldMenuJunctionRecipe.col * 80.0f, (f32)fldMenuJunctionRecipe.row * 48.0f + 24.0f);
    }
    FldMenuWinTextClear(8);
    FldMenuWinTextAdd(8, 2, sysGrxsysPac->msg->tbl[16], -0x30, -0x30, 0x20, 0x20, n++, 0x10, 1);
    /* goto-built input handling: the do-while(0) (loop weight: s7/s8 swap without
     * it), the never-taken zz back-edge to top and the out-of-line r1 block
     * all shape the original's allocation and block order (removing zz
     * changes 1,011 words) */
    do {
        if (!fldMenuJunctionRecipe.open) {
            top:
            if (padPort0[0].repeat & 0x1000) {
                goto r1;
            }
            if (padPort0[0].repeat & 0x4000) {
                if (fldMenuJunctionRecipe.row < nA - 1) {
                    fldMenuJunctionRecipe.row++;
                } else {
                    fldMenuJunctionRecipe.row = minA;
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            } else if (padPort0[0].repeat & 0x8000) {
                if (minB < fldMenuJunctionRecipe.col) {
                    fldMenuJunctionRecipe.col--;
                } else {
                    fldMenuJunctionRecipe.col = nB - 1;
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            } else if (padPort0[0].repeat & 0x2000) {
                if (fldMenuJunctionRecipe.col < nB - 1) {
                    fldMenuJunctionRecipe.col++;
                } else {
                    fldMenuJunctionRecipe.col = minB;
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            } else if (padPort0[0].trigger & 0x20) {
                if (itemWork->u76F8.junc.grid[fldMenuJunctionRecipe.col][fldMenuJunctionRecipe.row] & 0x80) {
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                    fldMenuJunctionRecipe.open = 1;
                    FldMenuWinOpen(9, 0x140, 0x100, 0x21C, 0x154, 0, 0, 0x400000);
                    FldMenuWinOpen(10, 0x140, 0x100, 0x21C, 0x154, 0, 0, 0x400000);
                    FldMenuWinOpen(11, 0x140, 0x100, 0x21C, 0x154, 0, 0, 0x400000);
                    FldMenuCursorHide();
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            } else if (padPort0[0].trigger & 0x40) {
                FldMenuClose();
            }
            zz = 0;
            if (zz) {
                goto top;
            }
        } else {
            if (padPort0[0].trigger & 0x40) {
                fldMenuJunctionRecipe.open = 0;
                FldMenuWinClose(9, 0, 0);
                FldMenuWinClose(10, 0, 0);
                FldMenuWinClose(11, 0, 0);
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            }
        }
        goto done;
        r1:
        if (minA < fldMenuJunctionRecipe.row) {
            fldMenuJunctionRecipe.row--;
        } else {
            fldMenuJunctionRecipe.row = nA - 1;
        }
        SndSePlay(0x7D1, 0, 0, 0x1E, 0);
    } while (0);
    done:
    ;
    s = FldMenuWinGetSprBuf(8, 4);
    h = FldMenuWinGetQuadBuf(8);
    FldMenuWinSetLayer(8, 4, &fldMenuTextures[1], s, 128.0f);
    sel = itemWork->u76F8.junc.grid[fldMenuJunctionRecipe.col][fldMenuJunctionRecipe.row];
    ci = sel & 0x3F;
    if (sel & 0x80) {
        nm0 = itemWork->u76F8.junc.ents[ci].unk8;
        ic0 = itemWork->u76F8.junc.ents[ci].unk3;
        FldMenuWinTextAdd(8, 2, nm0, 0x11E, -0x30, 0x14, 0x14, n++, -1, 1);
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0;
        s->v[0].x = 0x100;
        s->v[0].y = -0x30;
        s->v[1].x = 0x118;
        s->v[1].y = -0x18;
        s->v[0].u = menuItemIcons[ic0].u0;
        s->v[0].v = menuItemIcons[ic0].v0;
        s->v[1].u = menuItemIcons[ic0].u1;
        s->v[1].v = menuItemIcons[ic0].v1;
        s->z = 0;
        s++;
    } else {
        FldMenuWinTextAdd(8, 2, sysGrxsysPac->msg->tbl[16], 0x11E, -0x30, 0x14, 0x14, n++, 0x16, 2);
    }
    h2 = FldMenuBarBuild(h, 0xE0, -0x1E, 0x118, 0xC, 0xA);
    s->type = 0;
    h2->flag = 0;
    fldMenuJunctionRecipe.cell.y = minA * 0x30;
    for (i = minA; i < nA; i++) {
        fldMenuJunctionRecipe.cell.x = minB * 0x50;
        for (j = minB; j < nB; j++) {
            if (fldMenuJunctionRecipe.row == i && fldMenuJunctionRecipe.col == j) {
                if (fldMenuJunctionRecipe.alpha[cnt] < 0x80) {
                    fldMenuJunctionRecipe.alpha[cnt] += 2;
                    if (fldMenuJunctionRecipe.alpha[cnt] >= 0x80) {
                        fldMenuJunctionRecipe.alpha[cnt] = 0x80;
                    }
                }
            } else {
                if (fldMenuJunctionRecipe.alpha[cnt] >= 0x40) {
                    fldMenuJunctionRecipe.alpha[cnt] -= 2;
                    if (fldMenuJunctionRecipe.alpha[cnt] < 0x40) {
                        fldMenuJunctionRecipe.alpha[cnt] = 0x40;
                    }
                } else {
                    fldMenuJunctionRecipe.alpha[cnt] += 2;
                }
            }
            if (!(itemWork->u76F8.junc.grid[j][i] & 0x80)) {
                fldMenuJunctionRecipe.alpha[cnt] = 0x10;
            }
            fldMenuJunctionRecipe.cell.w = 0x50;
            fldMenuJunctionRecipe.cell.h = 0x30;
            fldMenuJunctionRecipe.cell.alpha = fldMenuJunctionRecipe.alpha[cnt++];
            FldMenuJunctionRecipeCellBuild(&fldMenuJunctionRecipe.cell);
            fldMenuJunctionRecipe.cell.x += 0x50;
        }
        fldMenuJunctionRecipe.cell.y += 0x30;
    }
    if (fldMenuJunctionRecipe.open) {
        FldMenuWinTextClear(9);
        FldMenuWinTextClear(10);
        FldMenuWinTextClear(11);
        fldMenuJunctionRecipe.cell.spr = FldMenuWinGetSprBuf(9, 1);
        fldMenuJunctionRecipe.cell.spr2 = FldMenuWinGetSprBuf(9, 2);
        t = FldMenuWinGetSprBuf(9, 3);
        s = FldMenuWinGetSprBuf(9, 4);
        s3 = FldMenuWinGetSprBuf(9, 5);
        h = FldMenuWinGetQuadBuf(9);
        FldMenuWinSetLayer(9, 1, FldOverlayImgGetTex(2), fldMenuJunctionRecipe.cell.spr, 128.0f);
        FldMenuWinSetLayer(9, 2, FldOverlayImgGetTex(3), fldMenuJunctionRecipe.cell.spr2, 128.0f);
        FldMenuWinSetLayer(9, 3, &fldMenuTextures[0], t, 128.0f);
        FldMenuWinSetLayer(9, 4, &fldMenuTextures[1], s, 128.0f);
        FldMenuWinSetLayer(9, 5, &fldMenuTextures[4], s3, 128.0f);
        fldMenuJunctionRecipe.cell.w = 0x21C;
        fldMenuJunctionRecipe.cell.h = 0x154;
        fldMenuJunctionRecipe.cell.alpha = 0x80;
        fldMenuJunctionRecipe.cell.x = 0;
        fldMenuJunctionRecipe.cell.y = 0;
        FldMenuJunctionRecipeCellBuild(&fldMenuJunctionRecipe.cell);
        sel = itemWork->u76F8.junc.grid[fldMenuJunctionRecipe.col][fldMenuJunctionRecipe.row];
        idx = sel & 0x3F;
        m = 0;
        if (sel & 0x80) {
            name = itemWork->u76F8.junc.ents[idx].unk8;
            icon = itemWork->u76F8.junc.ents[idx].unk3;
            FldMenuWinTextAdd(9, 0, name, 0x8E, 0x10, 0x14, 0x14, m++, -1, 10);
            s->type = 1;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0;
            s->v[0].x = 0x70;
            s->v[0].y = 0x10;
            s->v[1].x = 0x88;
            s->v[1].y = 0x28;
            s->v[0].u = menuItemIcons[icon].u0;
            s->v[0].v = menuItemIcons[icon].v0;
            s->v[1].u = menuItemIcons[icon].u1;
            s->v[1].v = menuItemIcons[icon].v1;
            s->z = 0;
            s++;
        } else {
            FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[16], 0x50, 0x10, 0x14, 0x14, m++, 0x16, 2);
        }
        FldMenuWinTextAdd(11, 2, sysGrxsysPac->msg->tbl[16], 0x30, 0x30, 0x10, 0x10, n++, 0x13, 10);
        /* floats declared before the u8 here only: EABI loads arguments in declaration order */
        t = ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberColorBuild)(t, (s16)itemWork->u76F8.junc.ents[idx].unk4, 0, 0xE0, 0x30, 1.0f, 0.8f, 1);
        FldMenuWinTextAdd(11, 2, sysGrxsysPac->msg->tbl[16], 0x110, 0x30, 0x10, 0x10, n++, 0x12, 10);
        t = FldMenuNumberColorBuild(t, itemWork->u76F8.junc.ents[idx].unk2, 0, 0x188, 0x30, 1, 1.0f, 0.8f);
        FldMenuWinTextAdd(11, 2, sysGrxsysPac->msg->tbl[16], 0xE0, 0x4C, 0x14, 0x14, n++, 0x15, 10);
        for (l = 0; l < itemWork->u76F8.junc.ents[idx].unk2; l++) {
            x4 = itemWork->u76F8.junc.ents[idx].s1[l].unk4;
            name = itemWork->u76F8.junc.ents[idx].s1[l].unk8;
            val = itemWork->u76F8.junc.ents[idx].s1[l].unk2;
            icon = itemWork->u76F8.junc.ents[idx].s1[l].unk1;
            if (l >= 3) {
                dy = (l - 3) * 0x14;
                FldMenuWinTextAdd(9, 0, name, 0x132, dy + 0x68, 0x10, 0x10, m++, -1, 10);
                if (x4) {
                    FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[9], 0x1DE, dy + 0x68, 0xE, 0x10, m++, x4, 10);
                }
                t = FldMenuNumberColorBuild(t, val, 0, 0x1BE, dy + 0x68, 1, 1.0f, 0.8f);
                s3->type = 1;
                s3->v[0].r = 0x80;
                s3->v[0].g = 0x80;
                s3->v[0].b = 0x80;
                s3->v[0].a = 0;
                s3->v[0].x = 0x11C;
                s3->v[0].y = dy + 0x68;
                s3->v[1].x = 0x130;
                s3->v[1].y = dy + 0x7C;
                s3->v[0].u = menuElementIcons[icon].u0;
                s3->v[0].v = menuElementIcons[icon].v0;
                s3->v[1].u = menuElementIcons[icon].u1;
                s3->v[1].v = menuElementIcons[icon].v1;
                s3->z = 0;
                s3++;
            } else {
                dy = l * 0x14;
                FldMenuWinTextAdd(9, 0, name, 0x32, dy + 0x68, 0x10, 0x10, m++, -1, 10);
                t = FldMenuNumberColorBuild(t, val, 0, 0xCC, dy + 0x68, 1, 1.0f, 0.8f);
                if (x4) {
                    FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[9], 0xEC, dy + 0x68, 0xE, 0x10, m++, x4, 10);
                }
                s3->type = 1;
                s3->v[0].r = 0x80;
                s3->v[0].g = 0x80;
                s3->v[0].b = 0x80;
                s3->v[0].a = 0;
                s3->v[0].x = 0x1C;
                s3->v[0].y = dy + 0x68;
                s3->v[1].x = 0x30;
                s3->v[1].y = dy + 0x7C;
                s3->v[0].u = menuElementIcons[icon].u0;
                s3->v[0].v = menuElementIcons[icon].v0;
                s3->v[1].u = menuElementIcons[icon].u1;
                s3->v[1].v = menuElementIcons[icon].v1;
                s3->z = 0;
                s3++;
            }
        }
        s3->type = 0;
        FldMenuWinTextAdd(11, 2, sysGrxsysPac->msg->tbl[16], 0xB0, 0xAC, 0x14, 0x14, n, 0x14, 10);
        for (l = 0; l < itemWork->u76F8.junc.ents[idx].unk1; l++) {
            name = itemWork->u76F8.junc.ents[idx].s2[l].unk4;
            icon = itemWork->u76F8.junc.ents[idx].s2[l].unk0;
            if (name) {
                FldMenuWinTextAdd(10, 1, name, 0x40, l * 0x14 + 0xCA, 0x10, 0x10, k++, -1, 10);
                s->type = 1;
                s->v[0].r = 0x80;
                s->v[0].g = 0x80;
                s->v[0].b = 0x80;
                s->v[0].a = 0;
                s->v[0].x = 0x2C;
                s->v[0].y = l * 0x14 + 0xCA;
                s->v[1].x = 0x3C;
                s->v[1].y = l * 0x14 + 0xDA;
                s->v[0].u = menuItemIcons[icon].u0;
                s->v[0].v = menuItemIcons[icon].v0;
                s->v[1].u = menuItemIcons[icon].u1;
                s->v[1].v = menuItemIcons[icon].v1;
                s->z = 0;
                s++;
            } else {
                FldMenuWinTextAdd(10, 1, sysGrxsysPac->msg->tbl[16], 0x40, l * 0x14 + 0xCA, 0x10, 0x10, k++, 0x16, 10);
            }
            FldMenuWinTextAdd(10, 1, sysGrxsysPac->msg->tbl[16], 0xF0, l * 0x14 + 0xCA, 0x10, 0x10, m++, 0xB, 10);
            name = itemWork->u76F8.junc.ents[idx].s2[l].unk8;
            icon = itemWork->u76F8.junc.ents[idx].s2[l].unk1;
            if (name) {
                FldMenuWinTextAdd(10, 1, name, 0x140, l * 0x14 + 0xCA, 0x10, 0x10, k++, -1, 10);
                s->type = 1;
                s->v[0].r = 0x80;
                s->v[0].g = 0x80;
                s->v[0].b = 0x80;
                s->v[0].a = 0;
                s->v[0].x = 0x12C;
                s->v[0].y = l * 0x14 + 0xCA;
                s->v[1].x = 0x13C;
                s->v[1].y = l * 0x14 + 0xDA;
                s->v[0].u = menuItemIcons[icon].u0;
                s->v[0].v = menuItemIcons[icon].v0;
                s->v[1].u = menuItemIcons[icon].u1;
                s->v[1].v = menuItemIcons[icon].v1;
                s->z = 10;
                s++;
            } else {
                FldMenuWinTextAdd(10, 1, sysGrxsysPac->msg->tbl[16], 0x140, l * 0x14 + 0xCA, 0x10, 0x10, k++, 0x16, 10);
            }
        }
        s->type = 0;
        s3->type = 0;
        t->type = 0;
        h->flag = 0;
    }
    return 0xC;
}
