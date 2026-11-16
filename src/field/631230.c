/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
/* rodata: 0x00675350-0x006753E8 */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "save.h"
#include "field/menu.h"
#include "field/n23.h"
#include "field/n24.h"

/* sprStarIconRects (sys.h SprRect, s16 w/h): the w/h reads here are (u16) - they only match unsigned */

/* The (u8)/(u16) casts on itemWork reads are this function's views: it reads
   ItemResist id/type/base/val, ItemStat.id and ItemPageSkill/ItemPageMove
   unk7/unkA/unkC unsigned. */

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);
void VuSinCosDeg(f32 *, f32 *, f32);

static const char save[] = "save";

s32 FldMenuStatusUpdate(void) {
    f32 hx;
    f32 hy;
    f32 sn;
    f32 cs;
    FldMenuQuad *t;
    s32 i;
    s32 n;
    s32 cnt;
    s32 k;
    s32 j;
    s32 idx;
    s32 v;
    s32 y;
    s32 m;
    s32 n2;
    s32 w;
    FldMenuQuad *s;
    FldMenuQuad *q;
    FldMenuQuad *h;
    f32 g;
    u16 bw;
    s32 e;
    s32 x;

    n = 0;
    m = 0x15;
    bw = (u16)sprStarIconRects[0].w;
    for (i = 0; i < 10; i++) {
        if (fldMenuStatus.blink == i) {
            if (fldMenuStatus.bar[i] < (s16)bw) {
                fldMenuStatus.bar[i] += 2;
                if (fldMenuStatus.bar[i] < (s16)bw) {
                    continue;
                }
            }
            fldMenuStatus.blink++;
            if (fldMenuStatus.blink == 9) {
                fldMenuStatus.blink = 0;
            }
        } else if (fldMenuStatus.bar[i] > (s16)bw / 2) {
            fldMenuStatus.bar[i]--;
        }
    }
    fldMenuStatus.timer++;
    if (fldMenuStatus.page != fldMenuStatus.pageNext || fldMenuStatus.chr != fldMenuStatus.chrNext) {
        s16 tbl[6][3] = {
            { 0x10E, 0x118, 0x21C }, { 0x10E, 0x118, 0x21C }, { 0xF0, 0xAA, 0x208 },
            { 0xE8, 0xBE, 0x208 }, { 0x10E, 0xC8, 0x1F4 }, { 0x10E, 0xC8, 0x1F4 },
        };
        u8 mode[4] = { 0, 1, 2, 3 };
        u8 eff[4] = { 1, 0, 3, 2 };

        FldMenuWinClose(fldMenuStatus.winPage + 10, 0, 1);
        FldMenuWinClose(fldMenuStatus.winPage + 8, 0, mode[fldMenuStatus.dir]);
        fldMenuStatus.winPage ^= 1;
        fldMenuStatus.chr = fldMenuStatus.chrNext;
        fldMenuStatus.page = fldMenuStatus.pageNext;
        fldMenuStatus.last = -1;
        fldMenuStatus.cursor = 0;
        fldMenuStatus.scroll = 0;
        fldMenuStatus.toggle = 0;
        for (i = 9; i >= 0; i--) {
            fldMenuStatus.gauge[i] = 0.0f;
        }
        FldMenuWinOpen(fldMenuStatus.winPage + 8, 0x140, tbl[fldMenuStatus.pageNext][0], tbl[fldMenuStatus.pageNext][2],
                      tbl[fldMenuStatus.pageNext][1], 0, eff[fldMenuStatus.dir], 0x89001);
        if (fldMenuStatus.page == 3) {
            FldMenuWinOpen(fldMenuStatus.winPage + 10, 0x1C0, 0x190, 0x118, 0x46, 0, 3, 9);
        }
    }
    idx = sysSaveData.party[fldMenuStatus.chr] - 1;
    ItemStatusParamsBuild(idx);
    if (fldMenuStatus.page >= 2) {
        FldMenuWinSetActive(fldMenuStatus.winPage + 8);
    } else {
        FldMenuWinSetActive(10);
    }
    j = fldMenuStatus.scroll;
    for (i = 0; i < 7; j++, i++) {
        k = 7 - i;
        w = k * 6;
        if (i == fldMenuStatus.cursor) {
            if ((fldMenuStatus.rowX[j] -= 8) < w) {
                fldMenuStatus.rowX[j] = w;
            }
        } else {
            if ((fldMenuStatus.rowX[j] += 8) >= w + 0x10) {
                fldMenuStatus.rowX[j] = w + 0x10;
            }
        }
    }
    FldMenuWinTextClear(fldMenuStatus.winPage + 8);
    FldMenuWinTextClear(fldMenuStatus.winPage + 10);
    w = TxtMsgGetWidth(sysGrxsysPac->msg->tbl[2], idx, 0x18);
    FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[2], 0x54 - w / 2, -4, 0x18, 0x14, n++,
                  idx, 1);
    fldMenuStatus.help = 0;
    FldMenuWinLrArrowsSet(fldMenuStatus.winPage + 8, 1, 12.0f);
    x = 1;

    switch (fldMenuStatus.page) {
    case 0:
        FldMenuCursorSetAt(0x11, 32.0f, 12.0f);
        fldMenuStatus.toggle = 0;
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[12], 0x18C, 0x11A, 0x18, 0x14, m++,
                      0, 6);
        FldMenuWinSetPortrait(fldMenuStatus.winPage + 8, fldMenuStatus.chr, (idx + 1) * 100, 4, 0xC);
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 0);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 0, &fldMenuTextures[2], s, 128.0f);
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0x80;
        s->v[0].x = 0x62;
        s->v[0].y = 0x1C;
        s->v[1].x = 0x80;
        s->v[1].y = 0x30;
        s->v[0].u = 0xEC;
        s->v[0].v = 0x5C;
        s->v[1].u = 0x100;
        s->v[1].v = 0x6A;
        s->z = 0;
        s++;
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0x80;
        s->v[0].x = 0x62;
        s->v[0].y = 0x34;
        s->v[1].x = 0x80;
        s->v[1].y = 0x48;
        s->v[0].u = 0xCE;
        s->v[0].v = 0x5A;
        s->v[1].u = 0xEC;
        s->v[1].v = 0x6E;
        s->z = 0;
        s++;
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0x80;
        s->v[0].x = 0x62;
        s->v[0].y = 0x4C;
        s->v[1].x = 0x80;
        s->v[1].y = 0x60;
        s->z = 0;
        s->v[0].u = 0xCE;
        s->v[0].v = 0x82;
        s->v[1].u = 0xEC;
        s->v[1].v = 0x96;
        s[1].type = 0;
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 1);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 1, &fldMenuTextures[1], s, 128.0f);
        h = FldMenuBarBuild(FldMenuWinGetQuadBuf(fldMenuStatus.winPage + 8), 8, 0x6C, 0xE0, 8, idx + 0x14);
        {
            s32 eq[6] = { 0x15, 0x1F, 0x27, 0x25, 0x2E, 0x30 };

            for (i = 0; i < 6; i++) {
                s32 r = eq[i];
                y = i * 30;
                s->type = 1;
                s->v[0].x = 0x118;
                s->v[0].r = 0x80;
                s->v[0].g = 0x80;
                s->v[0].b = 0x80;
                s->v[0].a = 0x80;
                s->v[0].y = y + 8;
                s->v[1].x = menuItemIcons[r].w + 0x118;
                s->v[1].y = y + (menuItemIcons[r].h + 8);
                s->z = 0;
                s->v[0].u = menuItemIcons[r].u0;
                s->v[0].v = menuItemIcons[r].v0;
                s->v[1].u = menuItemIcons[r].u1;
                s->v[1].v = menuItemIcons[r].v1;
                s++;
                if (itemWork->chars[fldMenuStatus.chr].equip[i].name != 0) {
                    FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, itemWork->chars[fldMenuStatus.chr].equip[i].name,
                                  0x136, y + 0xA, 0x12, 0x12, n++, -1, 0);
                }
                if (i == 0) {
                    h = FldMenuBarBuild(h, 0x118, 0x18, 0xFA, 8, 2);
                } else {
                    h = FldMenuBarBuild(h, 0x118, y + 0x18, 0xFA, 8, 1);
                }
            }
        }
        s->type = 0;
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 2);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 2, &fldMenuTextures[0], s, 128.0f);
        s = FldMenuNumberBuild(s, itemWork->chars[fldMenuStatus.chr].level, 0, 0x82, 0x1E, 1.0f, 0.8f, 0);
        s = FldMenuHpFractionBuild(s, itemWork->chars[fldMenuStatus.chr].hp, itemWork->chars[fldMenuStatus.chr].hpMax, 0x82, 0x36);
        s = FldMenuFractionBuild(s, itemWork->chars[fldMenuStatus.chr].sp, itemWork->chars[fldMenuStatus.chr].spMax, 0x82, 0x4E);
        s->type = 0;
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 3);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 3, &fldMenuTextures[0], s, 128.0f);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], 0x118, 0xD8, 0x14, 0x14, n,
                      0x15, 2);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], 0x1D6, 0xDA, 0x10, 0x10, n,
                      0x15, 0);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], 0x1D6, 0xF8, 0x10, 0x10, n++,
                      0x15, 0);
        s = FldMenuNumberBuild(s, itemWork->chars[fldMenuStatus.chr].unk8, 1, 0x1D2, 0xD8, 1.0f, 1.0f, 0);
        h = FldMenuBarBuild(h, 0x10E, 0xE8, 0x100, 8, 0xA);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], 0x118, 0xF6, 0x14, 0x14, n++,
                      0x16, 3);
        s = FldMenuNumberBuild(s, itemWork->chars[fldMenuStatus.chr].unkC, 1, 0x1D2, 0xF6, 1.0f, 1.0f, 0);
        h = FldMenuBarBuild(h, 0x10E, 0x106, 0x100, 8, 0xA);
        s->type = 0;
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 4);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 4, &fldMenuTextures[0], s, 128.0f);
        v = 0;
        for (i = 0; i < 6; i++) {
            s32 yy;

            switch (i) {
            case 0:
                v = itemWork->chars[fldMenuStatus.chr].stats[0];
                break;
            case 1:
                v = itemWork->chars[fldMenuStatus.chr].stats[1];
                break;
            case 2:
                v = itemWork->chars[fldMenuStatus.chr].stats[2];
                break;
            case 3:
                v = itemWork->chars[fldMenuStatus.chr].stats[3];
                break;
            case 4:
                v = itemWork->chars[fldMenuStatus.chr].stats[4];
                break;
            case 5:
                v = itemWork->chars[fldMenuStatus.chr].stats[5];
                break;
            }
            yy = i * 24 + 0x7C;
            FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], 4, yy, 0x14, 0x14, m++,
                          i + 2, 0);
            s = FldMenuNumberBuild(s, v, 0, 0x36, yy, 1.0f, 1.0f, 0);
        }
        s->type = 0;
        i = 0;
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 5);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 5, &fldMenuTextures[0], s, 128.0f);
        for (i = 0; i < 6; i++) {
            s32 yy;

            switch (i) {
            case 0:
                v = itemWork->chars[fldMenuStatus.chr].stats2[0];
                break;
            case 1:
                v = itemWork->chars[fldMenuStatus.chr].stats2[1];
                break;
            case 2:
                v = itemWork->chars[fldMenuStatus.chr].stats2[2];
                break;
            case 3:
                v = itemWork->chars[fldMenuStatus.chr].stats2[3];
                break;
            case 4:
                v = itemWork->chars[fldMenuStatus.chr].stats2[4];
                break;
            case 5:
                v = itemWork->chars[fldMenuStatus.chr].stats2[5];
                break;
            }
            yy = i * 24 + 0x7C;
            s = FldMenuNumberBuild(s, v, 0, 0xB6, yy, 1.0f, 1.0f, 0);
            FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], 0x6C, yy, 0xE, 0x14, n++,
                          i + 8, 0);
        }
        s->type = 0;
        break;
    case 1:
        FldMenuCursorSetAt(0x11, 32.0f, 12.0f);
        fldMenuStatus.toggle = 0;
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[12], 0x19C, 0x11A, 0x18, 0x14, n++,
                      1, 6);
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 1);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 1, &fldMenuTextures[6], s, 128.0f);
        t = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 0);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 0, &fldMenuTextures[4], t, 128.0f);
        h = FldMenuWinGetQuadBuf(fldMenuStatus.winPage + 8);
        q = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 2);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 2, &fldMenuTextures[8], q, 128.0f);
        ItemStatusResistBuild(fldMenuStatus.chr);
        ItemStatusParamsBuild(fldMenuStatus.chr);
        for (i = 0; i < 8; i++) {
            y = i * 24;
            k = (u8)itemWork->resists[i].id;
            t->v[0].x = 0x10;
            t->v[0].r = 0x80;
            t->v[0].g = 0x80;
            t->v[0].b = 0x80;
            t->v[0].a = 0x80;
            t->v[0].y = y + 0x20;
            t->v[1].x = menuElementIcons[k].w + 0x10;
            t->v[1].y = y + (menuElementIcons[k].h + 0x20);
            t->v[0].u = menuElementIcons[k].u0;
            t->v[0].v = menuElementIcons[k].v0;
            t->v[1].u = menuElementIcons[k].u1;
            t->v[1].v = menuElementIcons[k].v1;
            t->type = 1;
            t->z = 0;
            t++;
            g = (u16)itemWork->resists[i].val;
            if ((u16)itemWork->resists[i].base < 100.0f) {
                h = FldMenuBarBuild(h, (s32)((u16)itemWork->resists[i].base / 3.0f) + 0x80, y + 0x28, 0x21, 8, 2);
            }
            if (g > 100.0f) {
                h = FldMenuBarBuild(h, 0xA1, y + 0x28, (s32)((g - 100.0f) / 3.0f), 8, 1);
            }
            FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[10], 0x28, y + 0x22, 0x10, 0x14,
                          n++, (u8)itemWork->resists[i].type + 0x13, 2);
        }
        s->type = 0;
        for (i = 0; i < 10; i++) {
            f32 lm = 100.0f - itemWork->statList[i].val * 100.0f;

            if (fldMenuStatus.gauge[i] < lm) {
                fldMenuStatus.gauge[i] += 4.0f;
                if (fldMenuStatus.gauge[i] > lm) {
                    fldMenuStatus.gauge[i] = lm;
                }
            }
        }
        for (i = 0; i < 10; i++) {
            k = (u8)itemWork->statList[i].id - 1;
            g = 110.0f;
            VuSinCosDeg(&sn, &cs, (f32)i * 36.0f - 90.0f);
            s->type = 1;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0x80;
            s->v[0].x = (s32)(cs * g * 1.2f) + 0x14A;
            s->v[0].y = (s32)(sn * g) + 0x76;
            s->v[1].x = (s32)(cs * g * 1.2f) + (s16)(menuStatusIcons[k].w + 0x14A);
            s->v[1].y = (s32)(sn * g) + (s16)(menuStatusIcons[k].h + 0x76);
            s->z = 0;
            s->v[0].u = menuStatusIcons[k].u0;
            s->v[0].v = menuStatusIcons[k].v0;
            s->v[1].u = menuStatusIcons[k].u1;
            s->v[1].v = menuStatusIcons[k].v1;
            if (k == 6) {
                s32 r = menuStatusIcons[k].h + 0x76;

                FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[10],
                              (s32)(cs * g * 1.2f) + 0x138, (s32)(sn * g) + r, 0x10,
                              0x10, n++, i + 1, 0);
            } else {
                s32 r = menuStatusIcons[k].h + 0x76;

                FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[10],
                              (s32)(cs * g * 1.2f) + 0x14A, (s32)(sn * g) + r, 0x10,
                              0x10, n++, i + 1, 0);
            }
            {
                s32 r = menuStatusIcons[k].w + 0x14A;

                /* floats declared before the u8: EABI loads arguments in declaration order */
                q = ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuPercentBuild)(q, (s32)fldMenuStatus.gauge[i], 0, (s32)(cs * g * 1.2f) + r, (s32)(sn * g) + 0x7A, 1.0f, 1.0f, 0);
            }
            s++;
            h->type = 6;
            h->v[0].r = 0;
            h->v[0].g = 0;
            h->v[0].b = 0;
            h->v[0].a = 0x80;
            h->v[1].r = 0x80;
            h->v[1].g = 0x80;
            h->v[1].b = 0x80;
            h->v[1].a = 0x80;
            h->v[2].r = 0x80;
            h->v[2].g = 0x80;
            h->v[2].b = 0x80;
            h->v[2].a = 0x80;
            h->v[3].r = 0x80;
            h->v[3].g = 0x80;
            h->v[3].b = 0x80;
            h->v[3].a = 0x80;
            h->v[0].x = 0x168;
            h->v[0].y = 0x80;
            g = fldMenuStatus.gauge[i];
            h->v[1].x = (s32)(cs * g) + 0x164;
            h->v[1].y = (s32)(sn * g * 0.93f) + 0x80;
            VuSinCosDeg(&sn, &cs, (f32)(i + 1) * 36.0f - 90.0f);
            if (i == 9) {
                g = fldMenuStatus.gauge[0];
            } else {
                g = fldMenuStatus.gauge[i + 1];
            }
            h->v[2].x = h->v[3].x = (s32)(cs * g) + 0x164;
            h->v[2].y = h->v[3].y = (s32)(sn * g * 0.93f) + 0x80;
            h++;
        }
        s->type = 0;
        t->type = 0;
        h->type = 0;
        q->type = 0;
        break;
    case 2:
        if (padPort0[0].repeat & 0x1000) {
            FldMenuListCursorUp(&fldMenuStatus.cursor, &fldMenuStatus.scroll, 5, 0, itemWork->chars[fldMenuStatus.chr].skillCount);
        }
        if (padPort0[0].repeat & 0x4000) {
            FldMenuListCursorDown(&fldMenuStatus.cursor, &fldMenuStatus.scroll, 5, 0, itemWork->chars[fldMenuStatus.chr].skillCount);
        }
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[12], 0x17C, 0xAC, 0x18, 0x14, n++,
                      3, 6);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9], 0x124, -0x10, 0x10, 0x12, n++,
                      0xE, 2);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9], 0x168, -0x10, 0x10, 0x12, n++,
                      7, 2);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9], 0x1AC, -0x10, 0x12, 0x12, n++,
                      0xD, 2);
        FldMenuCursorSetAt(fldMenuStatus.winPage + 8,
                      (u32)((itemWork->chars[fldMenuStatus.chr].skillCount - fldMenuStatus.cursor) * 6) + 32.0f,
                      (u32)(fldMenuStatus.cursor * 24) + 32.0f);
        j = fldMenuStatus.scroll;
        h = FldMenuWinGetQuadBuf(fldMenuStatus.winPage + 8);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 0, &fldMenuTextures[3], h, 128.0f);
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 1);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 1, &fldMenuTextures[4], s, 128.0f);
        t = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 2);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 2, &fldMenuTextures[0], t, 128.0f);
        q = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 3);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 3, &fldMenuTextures[4], q, 128.0f);
        for (i = 0; i < itemWork->chars[fldMenuStatus.chr].skillCount; i++, j++) {
            if (itemWork->chars[fldMenuStatus.chr].skills[fldMenuStatus.scroll + fldMenuStatus.cursor].valid != 0) {
                if (fldMenuStatus.cursor == i) {
                    FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, itemWork->chars[fldMenuStatus.chr].skills[j].name,
                                  fldMenuStatus.rowX[j] + 0x2A, i * 24 + 0x1A, 0x13, 0x16, n++, -1, 8);
                } else {
                    FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, itemWork->chars[fldMenuStatus.chr].skills[j].name,
                                  fldMenuStatus.rowX[j] + 0x2A, i * 24 + 0x1A, 0x12, 0x14, n++, -1, 0);
                }
                h = FldMenuBarBuild(h, fldMenuStatus.rowX[j] + 8, i * 24 + 0x28, 0x1B8, 0xC, 2);
            } else {
                FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], fldMenuStatus.rowX[j] + 0x30,
                              i * 24 + 0x1A, 0x12, 0x14, n++, 0x17, 1);
                h = FldMenuBarBuild(h, 8, i * 24 + 0x28, 0x1B8, 0xC, 0xA);
            }
            y = i * 24;
            e = itemWork->chars[fldMenuStatus.chr].skills[j].icon;
            s->type = 1;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0;
            s->v[0].x = fldMenuStatus.rowX[j] + 0x10;
            s->v[0].y = y + 0x1A;
            s->v[1].x = fldMenuStatus.rowX[j] + (menuElementIcons[e].w + 0x10);
            s->v[1].y = y + (menuElementIcons[e].h + 0x1A);
            s->z = 0;
            s->v[0].u = menuElementIcons[e].u0;
            s->v[0].v = menuElementIcons[e].v0;
            s->v[1].u = menuElementIcons[e].u1;
            s->v[1].v = menuElementIcons[e].v1;
            s++;
            if ((u8)itemWork->chars[fldMenuStatus.chr].skills[j].unk7 != 0) {
                FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9], fldMenuStatus.rowX[j] + 0x12C,
                              y + 0x1C, 0x10, 0x14, n++, (u8)itemWork->chars[fldMenuStatus.chr].skills[j].unk7, 0);
            }
            if (fldMenuStatus.timer > 0x80) {
                if (itemWork->chars[fldMenuStatus.chr].skills[j].level != itemWork->chars[fldMenuStatus.chr].skills[j].max) {
                    for (w = 0; w < itemWork->chars[fldMenuStatus.chr].skills[j].level; w++) {
                        q->type = 1;
                        q->v[0].r = 0x80;
                        q->v[0].g = 0x80;
                        q->v[0].b = 0x80;
                        q->v[0].a = 0;
                        q->v[0].x = w * 12 + (s16)(fldMenuStatus.rowX[j] + 0x160);
                        q->v[0].y = i * 24 + 0x1B;
                        q->v[1].x = fldMenuStatus.bar[w] + w * 12 + (s16)(fldMenuStatus.rowX[j] + 0x160);
                        q->v[1].y = i * 24 + (fldMenuStatus.bar[w] + 0x1B);
                        q->z = 0;
                        q->v[0].u = sprStarIconRects[0].u0;
                        q->v[0].v = sprStarIconRects[0].v0;
                        q->v[1].u = sprStarIconRects[0].u1;
                        q->v[1].v = sprStarIconRects[0].v1;
                        q++;
                    }
                } else {
                    q->type = 1;
                    q->v[0].r = 0x80;
                    q->v[0].g = 0x80;
                    q->v[0].b = 0x80;
                    q->v[0].a = 0;
                    q->v[0].x = fldMenuStatus.rowX[j] + 0x160;
                    q->v[0].y = y + 0x1B;
                    q->v[1].x = fldMenuStatus.rowX[j] + ((u16)sprStarIconRects[0].w + 0x160);
                    q->v[1].y = y + ((u16)sprStarIconRects[0].h + 0x1B);
                    q->z = 0;
                    q->v[0].u = sprStarIconRects[0].u0;
                    q->v[0].v = sprStarIconRects[0].v0;
                    q->v[1].u = sprStarIconRects[0].u1;
                    q->v[1].v = sprStarIconRects[0].v1;
                    q++;
                    q->type = 1;
                    q->v[0].r = 0x80;
                    q->v[0].g = 0x80;
                    q->v[0].b = 0x80;
                    q->v[0].a = 0;
                    q->v[0].x = fldMenuStatus.rowX[j] + 0x178;
                    q->v[0].y = y + 0x19;
                    q->v[1].x = fldMenuStatus.rowX[j] + ((u16)sprStarIconRects[1].w + 0x178);
                    q->v[1].y = y + ((u16)sprStarIconRects[1].h + 0x19);
                    q->z = 0;
                    q->v[0].u = sprStarIconRects[1].u0;
                    q->v[0].v = sprStarIconRects[1].v0;
                    q->v[1].u = sprStarIconRects[1].u1;
                    q->v[1].v = sprStarIconRects[1].v1;
                    q++;
                }
            } else {
                t = FldMenuFractionBuild(t, (u16)itemWork->chars[fldMenuStatus.chr].skills[j].unkA,
                                  (u16)itemWork->chars[fldMenuStatus.chr].skills[j].unkC, fldMenuStatus.rowX[j] + 0x160, y + 0x1E);
            }
            t = FldMenuNumberBuild(t, itemWork->chars[fldMenuStatus.chr].skills[j].unk4, 1, fldMenuStatus.rowX[j] + 0x120, i * 24 + 0x1E, 1.0f, 0.8f, 0);
        }
        s->type = 0;
        t->type = 0;
        h->type = 0;
        q->type = 0;
        if (itemWork->chars[fldMenuStatus.chr].skills[fldMenuStatus.scroll + fldMenuStatus.cursor].valid != 0) {
            fldMenuStatus.help = itemWork->chars[fldMenuStatus.chr].skills[fldMenuStatus.scroll + fldMenuStatus.cursor].help;
        }
        break;
    case 3:
        if (padPort0[0].repeat & 0x1000) {
            FldMenuListCursorUp(&fldMenuStatus.cursor, &fldMenuStatus.scroll, 6, 3, itemWork->chars[fldMenuStatus.chr].unk46);
        }
        if (padPort0[0].repeat & 0x4000) {
            FldMenuListCursorDown(&fldMenuStatus.cursor, &fldMenuStatus.scroll, 6, 3, itemWork->chars[fldMenuStatus.chr].unk46);
        }
        cnt = itemWork->chars[fldMenuStatus.chr].unk46;
        {
            s32 up;
            s32 down;

            if (fldMenuStatus.scroll != 0) {
                up = 1;
            } else {
                up = 0;
            }
            if (cnt >= 7) {
                if (fldMenuStatus.scroll + 6 < cnt) {
                    down = 1;
                } else {
                    down = 0;
                }
            } else {
                down = 0;
            }
            FldMenuWinSetScrollArrows(fldMenuStatus.winPage + 8, up, down);
        }
        if (cnt >= 6) {
            cnt = 6;
        }
        FldMenuCursorSetAt(fldMenuStatus.winPage + 8, (u32)((cnt - fldMenuStatus.cursor) * 6) + 32.0f,
                      (u32)(fldMenuStatus.cursor * 24) + 32.0f);
        n2 = 7;
        j = fldMenuStatus.scroll;
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[12], 0x15E, 0xC0, 0x16, 0x14, n2++,
                      4, 6);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9], 0x124, -0x10, 0x10, 0x12, m++,
                      0xE, 2);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9], 0x168, -0x10, 0x10, 0x12, m++,
                      7, 2);
        FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9], 0x1D4, -0x10, 0x12, 0x12, m++,
                      0xF, 2);
        h = FldMenuWinGetQuadBuf(fldMenuStatus.winPage + 8);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 0, &fldMenuTextures[3], h, 128.0f);
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 1);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 1, &fldMenuTextures[4], s, 128.0f);
        t = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 2);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 2, &fldMenuTextures[0], t, 128.0f);
        q = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 8, 3);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 8, 3, &fldMenuTextures[4], q, 128.0f);
        for (i = 0; i < cnt; i++, j++) {
            if (itemWork->chars[fldMenuStatus.chr].moves[j].valid != 0) {
                if (fldMenuStatus.cursor == i) {
                    FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, itemWork->chars[fldMenuStatus.chr].moves[j].name,
                                  fldMenuStatus.rowX[j] + 0x30, i * 24 + 0x1A, 0x13, 0x16, n++, -1, 8);
                } else {
                    FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, itemWork->chars[fldMenuStatus.chr].moves[j].name,
                                  fldMenuStatus.rowX[j] + 0x30, i * 24 + 0x1A, 0x12, 0x14, n++, -1, 0);
                }
                h = FldMenuBarBuild(h, fldMenuStatus.rowX[j] + 8, (y = i * 24) + 0x28, 0x1B8, 0xC, 2);
                e = itemWork->chars[fldMenuStatus.chr].moves[j].icon;
                s->type = 1;
                s->v[0].r = 0x80;
                s->v[0].g = 0x80;
                s->v[0].b = 0x80;
                s->v[0].a = 0;
                s->v[0].x = fldMenuStatus.rowX[j] + 0x10;
                s->v[0].y = y + 0x1A;
                s->v[1].x = fldMenuStatus.rowX[j] + (menuElementIcons[e].w + 0x10);
                s->v[1].y = y + (menuElementIcons[e].h + 0x1A);
                s->z = 0;
                s->v[0].u = menuElementIcons[e].u0;
                s->v[0].v = menuElementIcons[e].v0;
                s->v[1].u = menuElementIcons[e].u1;
                s->v[1].v = menuElementIcons[e].v1;
                s++;
                if ((u8)itemWork->chars[fldMenuStatus.chr].moves[j].unk7 != 0) {
                    FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[9],
                                  fldMenuStatus.rowX[j] + 0x12C, y + 0x1C, 0x10, 0x14, n2++, (u8)itemWork->chars[fldMenuStatus.chr].moves[j].unk7, 0);
                }
                if (itemWork->chars[fldMenuStatus.chr].moves[j].level != itemWork->chars[fldMenuStatus.chr].moves[j].max) {
                    for (w = 0; w < itemWork->chars[fldMenuStatus.chr].moves[j].level; w++) {
                        q->type = 1;
                        q->v[0].r = 0x80;
                        q->v[0].g = 0x80;
                        q->v[0].b = 0x80;
                        q->v[0].a = 0;
                        q->v[0].x = w * 12 + (s16)(fldMenuStatus.rowX[j] + 0x160);
                        q->v[0].y = i * 24 + 0x1B;
                        q->v[1].x = fldMenuStatus.bar[w] + w * 12 + (s16)(fldMenuStatus.rowX[j] + 0x160);
                        q->v[1].y = i * 24 + (fldMenuStatus.bar[w] + 0x1B);
                        q->z = 0;
                        q->v[0].u = sprStarIconRects[0].u0;
                        q->v[0].v = sprStarIconRects[0].v0;
                        q->v[1].u = sprStarIconRects[0].u1;
                        q->v[1].v = sprStarIconRects[0].v1;
                        q++;
                    }
                } else {
                    q->type = 1;
                    q->v[0].r = 0x80;
                    q->v[0].g = 0x80;
                    q->v[0].b = 0x80;
                    q->v[0].a = 0;
                    q->v[0].x = fldMenuStatus.rowX[j] + 0x160;
                    q->v[0].y = y + 0x1B;
                    q->v[1].x = fldMenuStatus.rowX[j] + ((u16)sprStarIconRects[0].w + 0x160);
                    q->v[1].y = y + ((u16)sprStarIconRects[0].h + 0x1B);
                    q->z = 0;
                    q->v[0].u = sprStarIconRects[0].u0;
                    q->v[0].v = sprStarIconRects[0].v0;
                    q->v[1].u = sprStarIconRects[0].u1;
                    q->v[1].v = sprStarIconRects[0].v1;
                    q++;
                    q->type = 1;
                    q->v[0].r = 0x80;
                    q->v[0].g = 0x80;
                    q->v[0].b = 0x80;
                    q->v[0].a = 0;
                    q->v[0].x = fldMenuStatus.rowX[j] + 0x178;
                    q->v[0].y = y + 0x19;
                    q->v[1].x = fldMenuStatus.rowX[j] + ((u16)sprStarIconRects[1].w + 0x178);
                    q->v[1].y = y + ((u16)sprStarIconRects[1].h + 0x19);
                    q->z = 0;
                    q->v[0].u = sprStarIconRects[1].u0;
                    q->v[0].v = sprStarIconRects[1].v0;
                    q->v[1].u = sprStarIconRects[1].u1;
                    q->v[1].v = sprStarIconRects[1].v1;
                    q++;
                }
                t = FldMenuNumberBuild(t, itemWork->chars[fldMenuStatus.chr].moves[j].unk4, 1, fldMenuStatus.rowX[j] + 0x120, i * 24 + 0x1E, 1.0f, 0.8f, 0);
            } else {
                FldMenuWinTextAdd(fldMenuStatus.winPage + 8, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], fldMenuStatus.rowX[j] + 0x30,
                              i * 24 + 0x1A, 0x14, 0x14, n++, 0x18, 0);
                h = FldMenuBarBuild(h, fldMenuStatus.rowX[j] + 8, i * 24 + 0x28, 0x1A4, 0xC, 0xA);
            }
        }
        s->type = 0;
        t->type = 0;
        h->type = 0;
        q->type = 0;
        s = FldMenuWinGetSprBuf(fldMenuStatus.winPage + 10, 0);
        FldMenuWinSetLayer(fldMenuStatus.winPage + 10, 0, &fldMenuTextures[6], s, 128.0f);
        if (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].valid != 0) {
            u8 pos[4][2] = { { 8, 4 }, { 0x94, 4 }, { 8, 0x26 }, { 0x94, 0x26 } };

            for (i = 0; i < 4; i++) {
                if (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].slot[i][0] != 0) {
                    FldMenuWinTextAdd(fldMenuStatus.winPage + 10, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[2], pos[i][0] + 0x20,
                                  pos[i][1] + 8, 0x10, 0x10, m++, itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].slot[i][0] - 1, 1);
                    s->type = 1;
                    s->v[0].r = 0x80;
                    s->v[0].g = 0x80;
                    s->v[0].b = 0x80;
                    s->v[0].a = 0;
                    s->v[0].x = pos[i][0];
                    s->v[0].y = pos[i][1];
                    s->v[1].x = pos[i][0] + 0x1A;
                    s->v[1].y = pos[i][1] + 0x1A;
                    s->z = 0;
                    if (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].slot[i][0] - 1 >= 4) {
                        s->v[0].u = (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].slot[i][0] - 5) * 0x36;
                        s->v[0].v = 0x36;
                        s->v[1].u = (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].slot[i][0] - 5) * 0x36 + 0x36;
                        s->v[1].v = 0x6C;
                    } else {
                        s->v[0].u = (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].slot[i][0] - 1) * 0x36;
                        s->v[0].v = 0;
                        s->v[1].u = (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].slot[i][0] - 1) * 0x36 + 0x36;
                        s->v[1].v = 0x36;
                    }
                    s++;
                }
            }
        } else {
            FldMenuWinTextAdd(fldMenuStatus.winPage + 10, fldMenuStatus.winPage, sysGrxsysPac->msg->tbl[8], 0x40, 0x18, 0x14, 0x14, m,
                          0x18, 1);
        }
        s->type = 0;
        if (itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].valid != 0) {
            fldMenuStatus.help = itemWork->chars[fldMenuStatus.chr].moves[fldMenuStatus.scroll + fldMenuStatus.cursor].help;
        }
        break;
    }
    FldMenuCursorGetPos(&hx, &hy);
    if (FldMenuWinGetState(fldMenuStatus.winPage + 8) != 4) {
        fldMenuStatus.help = 0;
        fldMenuStatus.last = -1;
    }
    if (fldMenuStatus.help != 0) {
        if (fldMenuStatus.last != fldMenuStatus.cursor + fldMenuStatus.scroll) {
            fldMenuStatus.last = fldMenuStatus.cursor + fldMenuStatus.scroll;
            FldMenuCursorGetPos(&hx, &hy);
            if (fldMenuStatus.toggle != 0) {
                MenuHelpOpen(0, 1, fldMenuStatus.help, hx, hy + 16.0f);
            } else {
                MenuHelpOpen(1, 1, fldMenuStatus.help, 32.0f, 348.0f);
            }
        }
    } else {
        fldMenuStatus.last = -1;
        MenuHelpClose();
    }
    if (padPort0[0].trigger & 0x40) {
        SndSePlay(0x7D3, 0, 0, 0x1E, 0);
        return 1;
    }
    if (padPort0[0].trigger & 4) {
        if (fldMenuStatus.page != 0) {
            fldMenuStatus.pageNext--;
        } else {
            fldMenuStatus.pageNext = 3;
        }
        fldMenuStatus.dir = 0;
        MenuLrArrowsFlash(1);
        SndSePlay(0x7D5, 0, 0, 0x1E, 0);
        return 8;
    }
    if (padPort0[0].trigger & 8) {
        if (fldMenuStatus.page < 3) {
            fldMenuStatus.pageNext++;
        } else {
            fldMenuStatus.pageNext = 0;
        }
        fldMenuStatus.dir = 1;
        MenuLrArrowsFlash(0);
        SndSePlay(0x7D5, 0, 0, 0x1E, 0);
        return 8;
    }
    if (padPort0[0].trigger & 0x10) {
        if (fldMenuStatus.toggle != 0) {
            fldMenuStatus.toggle = 0;
        } else {
            fldMenuStatus.toggle = 1;
        }
        fldMenuStatus.last = -1;
    }
    return 8;
}
