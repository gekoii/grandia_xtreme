/* as: ee-gcc2.96-hazard */
/* rodata: 0x00675470-0x00675670 */
#include "common.h"
#include "grxsys.h"
#include "pad.h"
#include "field/n24.h"
#include "field/n23.h"

/* menuSkillTypeIcons, sprStarIconRects (sys.h SprRect, s16 w/h): the w/h reads here are (u16) - they only match unsigned */

/* Local prototypes, not protos/field.h: FldMenuShopListBuild tail-calls
 * ItemShopListBuild with a second argument the definition (s16) lacks, and a
 * cast call would block the tail call; FldMenuShopUpdate also differs (19
 * words) with the header and casts. */
u8 TxtMsgGetLength(s32, s32);
s32 TxtMsgGetWidth(s32, s32, s32);
s32 ItemStorageDeposit(s16, s16);
s32 ItemStorageWithdraw(s16, s16);
void ItemSetSold(s16);
s32 ItemBagRemove(u16, u8);
s32 ItemCanAdd(s16);
s32 ItemAdd(s16);
s32 ItemBookCheckDiscard(u16);
s32 ItemBookDiscard(u16);
s32 ItemSkillCheckDiscard(u16);
s32 ItemSkillDiscard(u16);
void PtySetGold(u32);
u32 PtyGetShopPoints(void);
void PtySetShopPoints(u32);
void ItemCharSlotListBuild(s16);
void ItemCharBookSlotGetType(s16, s16);
s32 ItemCharEquipBook(s16, s16, s16);
s32 ItemCharEquipSkill(s16, s16, s16, s16);
s32 ItemCharEquipEgg(s16, s16, s16);
s32 ItemCharUnequipBook(s16, s16);
s32 ItemCharUnequipEgg(s16, s16);
s32 ItemCharUnequipSkill(s16, s16, s16);
s32 ItemCharUnequipAllEggs(s16);
s32 ItemCharUnequipAllBooks(s16);
s32 ItemCharUnequipAllSkills(s16);
s32 ChrIsInParty(u8);
void FldMenuGuideSet(s32);
void FldMenuClose(void);
void FldMenuWinSetScrollArrows(u8, s32, s32);
void FldMenuCursorSetAt(u8, f32, f32);
FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
FldMenuQuad *FldMenuWinGetQuadBuf(u8);
void FldMenuCursorGetPos(f32 *, f32 *);
void FldMenuWinTextClear(u8);
void FldMenuWinTextAddIndexed(u8, u8, s32, s32, s32, s32, s32, s32, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);
void FldMenuWinOpen(u8, s16, s16, u16, u16, s32, s32, s32);
void FldMenuWinSetLayer(u8, s32, void *, FldMenuQuad *, f32);
s32 FldMenuWinGetState(u8);
void FldMenuWinLrArrowsSet(u8, s32, f32);
void FldMenuWinClose(u8, s32, s32);
void FldMenuWinSetActive(u8);
FldMenuQuad *FldMenuFractionBuild(FldMenuQuad *, s32, s32, s32, s32);
FldMenuQuad *FldMenuHpFractionBuild(FldMenuQuad *, s32, s32, s32, s32);
FldMenuQuad *FldMenuTaggedNumberBuild(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32, s32);
FldMenuQuad *FldMenuNumberBuild(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32);
FldMenuQuad *FldMenuNumber2Build(FldMenuQuad *, s32, s32, s32, s32, f32, f32, u8);
void FldMenuListFixOnRemove(u32 *, u32 *, s32, s32, s32);
void FldMenuListCursorDown(u32 *, u32 *, s32, s32, s32);
void FldMenuListCursorUp(u32 *, u32 *, s32, s32, s32);
FldMenuQuad *FldMenuBarBuild(FldMenuQuad *, s32, s32, s32, s32, s32);
FldMenuQuad *FldMenuShopCharPlateBuild(FldMenuQuad *, u8, s32, s32, s8, f32, f32);
f32 FldMenuWobbleCurve(f32);
void FldMenuShopListBuild(void);
void *ChrPortraitGetTex(s32);
void MenuLrArrowsFlash(s8);
void MenuHelpOpen(s32, s8, s32, f32, f32);
void MenuHelpClose(void);
void VuSinCosDeg(f32 *, f32 *, f32);
s32 SndSePlay(s32, s32, s32, s32, s32);

/* Not referenced by any code; it starts this file's .rodata, which the jump tables keep 16-aligned. */
static const char fldMenuUnusedSaveStrs[7][8] = { "save", "save", "save", "save", "save", "save", "save" };

extern FldMenuQuad fldMenuShopCharPlates[][2];

FldMenuQuad *FldMenuShopCharPlateBuild(FldMenuQuad *spr, u8 idx, s32 x, s32 y, s8 c, f32 sx, f32 sy) {
    u8 tbl[8][2] = {
        {0x14, 0x30}, {0x14, 0x20}, {0x14, 0x30}, {0x14, 0x20},
        {0x14, 0x30}, {0x14, 0x20}, {0x14, 0x30}, {0x14, 0x30},
    };
    FldMenuQuad *s;
    f32 h, w;
    f32 top;
    s32 y2, x2;

    spr->v[0].r = 0x80;
    w = sx * 98.0f;
    h = sy * 38.0f;
    top = y - h * 0.5f;
    spr->v[0].y = top;
    spr->v[0].g = 0x80;
    spr->v[0].x = x;
    spr->v[1].x = x + w;
    spr->v[1].y = top + h;
    s = fldMenuShopCharPlates[idx];
    spr->v[0].a = 0;
    spr->type = 1;
    x2 = x + 4;
    spr->v[1].u = 98;
    spr->v[0].b = 0x80;
    spr->v[1].v = 38;
    spr->z = 0;
    spr->v[0].u = 0;
    spr->v[0].v = 0;
    s->v[0].r = c * 3 - 0x80;
    h = sy * 30.0f;
    y2 = y - h * 0.5f;
    s->type = 1;
    s->v[0].g = c * 3 - 0x80;
    w = sx * 84.0f;
    spr++;
    s->v[0].a = 0;
    s->v[0].x = x2;
    s->v[0].y = y2;
    s->v[1].x = x2 + w;
    s->v[0].u = tbl[idx][0];
    s->v[0].b = c * 3 - 0x80;
    s->v[0].v = tbl[idx][1];
    s->v[1].y = y2 + h;
    s->v[1].u = tbl[idx][0] + 0x6C;
    s->z = 0;
    s->v[1].v = tbl[idx][1] + 0x24;
    spr->type = 0;
    s[1].type = 0;
    return spr;
}

void VuBezier3(FVECTOR *, FVECTOR *, f32);

f32 FldMenuWobbleCurve(f32 t) {
    f32 tbl[4][2] = {{0.0f, 1.0f}, {1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 0.0f}};
    FVECTOR out;
    FVECTOR m[4];
    s32 i;

    if (t >= 3.0f) {
        i = 3;
        t -= 3.0f;
    } else if (t >= 2.0f) {
        i = 2;
        t -= 2.0f;
    } else if (t >= 1.0f) {
        i = 1;
        t -= 1.0f;
    } else {
        i = 0;
    }
    m[0][0] = tbl[i][0];
    m[1][0] = tbl[i][0];
    m[2][0] = tbl[i][1];
    m[3][0] = tbl[i][1];
    VuBezier3(&out, m, t);
    return out[0];
}

void ItemShopListBuild(s16, s32);
void ItemCharSlotListBuild(s16);

void FldMenuShopListBuild(void) {
    switch (fldMenuShop.mode) {
    case 0:
        switch (fldMenuShop.tab) {
        case 0:
            ItemShopListBuild(0x16, 0);
            break;
        case 1:
            ItemShopListBuild(fldMenuShop.sel + 0x1E, 0);
            break;
        case 2:
            ItemShopListBuild(fldMenuShop.sel + 0x26, 0);
            break;
        case 3:
            ItemShopListBuild(fldMenuShop.sel + 0x2E, 0);
            break;
        }
        break;
    case 1:
        switch (fldMenuShop.tab) {
        case 0:
            ItemShopListBuild(fldMenuShop.sel, 0);
            break;
        case 1:
            ItemShopListBuild(fldMenuShop.sel + 8, 0);
            break;
        case 2:
            ItemShopListBuild(fldMenuShop.sel + 0x10, 0);
            break;
        case 3:
            ItemShopListBuild(0x15, 0);
            break;
        }
        break;
    case 2:
        switch (fldMenuShop.tab) {
        case 0:
            ItemShopListBuild(0x19, 0);
            break;
        case 1:
            ItemShopListBuild(fldMenuShop.sel + 0x17, 0);
            break;
        }
        break;
    case 3:
        ItemCharSlotListBuild(fldMenuShop.chr);
        break;
    }
}

#define MN fldMenuShop
#define W itemWork
#define FONT (sysGrxsysPac->msg)
#define SE(x) SndSePlay((x), 0, 0, 0x1E, 0)
s32 FldMenuShopUpdate(void) {
    s16 diff[8];
    FldMenuQuad *p;
    FldMenuQuad *s2;
    FldMenuQuad *s4;
    s32 nChr;
    s32 n8;
    s32 n13;
    s32 n11;
    s32 icon;
    s32 n;
    s32 m;
    s32 name;
    s32 flag;
    s32 b;
    s32 sub;
    u32 gold;
    s32 eq6;
    s32 i;
    s32 j;
    s32 k;
    s32 ok;
    s32 r;
    FldMenuQuad *s3;
    FldMenuQuad *s5;
    f32 sn;
    f32 cs;
    f32 sn2;
    f32 cs2;
    f32 hx;
    f32 hy;
    f32 sn3;
    f32 cs3;
    f32 f;
    s32 w;
    u32 d;
    s32 v;
    s32 lt;
    s32 rt;
    s32 lt8;

    nChr = MN.nChr;
    n8 = 0;
    n13 = 0;
    b = MN.buf ^ 1;
    n11 = 5;
    sub = 0;
    for (j = 0; j < 8; j++) {
        diff[j] = 0;
        MN.flags[j] = 0;
    }
    MN.ang2 += 5;
    if (MN.ang2 >= 360) {
        MN.ang2 -= 360;
    }
    if (MN.mode != 2) {
        gold = W->gold;
    } else {
        gold = PtyGetShopPoints();
    }
    for (i = 0; i < 10; i++) {
        if (MN.blink == i) {
            if (MN.bar[i] < (s16)sprStarIconRects[0].w) {
                MN.bar[i] += 2;
                if (MN.bar[i] < (s16)sprStarIconRects[0].w) {
                    continue;
                }
            }
            MN.blink++;
            if (MN.blink == 9) {
                MN.blink = 0;
            }
        } else if (MN.bar[i] > (s16)sprStarIconRects[0].w / 2) {
            MN.bar[i]--;
        }
    }
    MN.unk227++;
    if (MN.unk227 >= 40) {
        MN.unk227 = 0;
    }
    MN.ang += 5;
    if (MN.ang >= 360) {
        MN.ang -= 360;
    }
    switch (MN.state) {
    case 0: {
        u8 cnt[4][4] = { { 1, 8, 8, 5 }, { 8, 8, 5, 1 }, { 1, 2, 0, 0 }, { 1, 1, 5, 0 } };

        if (MN.state == 2 && MN.mode == 3) {
            n = 3;
        } else if (MN.mode == 3 && MN.tab == 2) {
            FldMenuShopListBuild();
            n = 1;
        } else {
            n = cnt[MN.mode][MN.tab];
        }
        if (MN.sel >= n) {
            MN.sel = 0;
        }
        if (padPort0[0].trigger & 0x40) {
            if (MN.mode != 3) {
                MN.state = 4;
                FldMenuShopListBuild();
                switch (MN.mode) {
                case 0:
                    FldMenuWinOpen(0xE, 0x140, 0xE0, 0xC8, 0x80, 0, 2, 0x80F0);
                    break;
                case 1:
                    FldMenuWinOpen(0xE, 0x154, 0xE0, 0xF0, 0x80, 0, 2, 0x80F0);
                    break;
                case 2:
                    FldMenuWinOpen(0xE, 0x14A, 0xF0, 0x104, 0x50, 0, 2, 0x80F0);
                    break;
                case 3:
                    FldMenuWinOpen(0xE, 0x140, 0xF0, 0xE0, 0x64, 0, 2, 0x80F0);
                    break;
                }
                FldMenuWinSetActive(0xE);
                FldMenuGuideSet(0xB);
            } else {
                MN.state = 2;
                FldMenuShopListBuild();
                FldMenuWinSetActive(0xA);
                FldMenuGuideSet(0x11);
            }
            MN.sel = 0;
            MN.cursor = 0;
            MN.scroll = 0;
            SE(0x7D3);
            MN.last = -1;
            break;
        }
        switch (MN.mode) {
        case 0:
        case 1:
        default:
            m = W->unk110E;
            break;
        case 2:
            switch (MN.tab) {
            case 0:
            default:
                m = W->unk110E;
                break;
            case 1:
                if (MN.sel == 0) {
                    m = W->counts[3];
                } else {
                    m = W->counts[2];
                }
                break;
            }
            break;
        case 3:
            switch (MN.tab) {
            case 0:
            default:
                m = W->unk6EE0;
                break;
            case 1:
                m = W->unk6EE2;
                break;
            case 2:
                m = W->unk76F4;
                break;
            }
            break;
        }
        if (padPort0[0].trigger & 0x20) {
            if (m == 0) {
                break;
            }
            switch (MN.mode) {
            case 0:
                FldMenuWinOpen(0xD, 0x140, 0xE0, 0x140, 0xA8, 0, 1, 0x80F0);
                FldMenuWinSetActive(0xD);
                MN.cursor2 = 1;
                MN.state = 1;
                SE(0x7D2);
                break;
            case 1:
                switch (MN.tab) {
                case 0:
                case 1:
                case 2:
                    if (ItemStorageWithdraw(W->shop[MN.scroll + MN.cursor].id, 1) >= 0) {
                        SE(0x7D2);
                    } else {
                        SE(0x7D4);
                    }
                    break;
                case 3:
                    if (ItemStorageDeposit(W->shop[MN.scroll + MN.cursor].unkA, 1) >= 0) {
                        SE(0x7D2);
                    } else {
                        SE(0x7D4);
                    }
                    break;
                }
                break;
            case 2:
                FldMenuWinOpen(0xD, 0x140, 0xE0, 0x140, 0xA8, 0, 1, 0x80F0);
                FldMenuWinSetActive(0xD);
                MN.cursor2 = 1;
                MN.state = 5;
                SE(0x7D2);
                break;
            case 3:
                switch (MN.tab) {
                case 0:
                    m = W->unk6ED4;
                    break;
                case 1:
                    m = W->books3Count;
                    break;
                case 2:
                    ItemCharBookSlotGetType(MN.chr, W->u76F8.recs[MN.scroll + MN.cursor].id);
                    ItemCharSlotListBuild(MN.chr);
                    m = W->books4Count;
                    break;
                }
                if (m != 0) {
                    FldMenuWinOpen(0xD, 0x140, 0xE0, 0x1AE, 0xC8, 0, 1, 0x80F0);
                    FldMenuWinSetActive(0xD);
                    MN.state = 3;
                    MN.cursor2 = 0;
                    MN.scroll2 = 0;
                    SE(0x7D2);
                } else {
                    SE(0x7D4);
                }
                break;
            }
            FldMenuShopListBuild();
            MN.last = -1;
        } else if (n >= 2 && (padPort0[0].trigger & 8)) {
            if (MN.sel < n - 1) {
                MN.sel++;
            } else {
                MN.sel = 0;
            }
            FldMenuWinClose(MN.buf + 8, 0, 1);
            MN.buf ^= 1;
            FldMenuWinOpen(MN.buf + 8, 0x184, 0xCC, 0x1B8, 0xBE, 0, 0, 0x48010);
            FldMenuWinSetActive(MN.buf + 8);
            MenuLrArrowsFlash(0);
            MN.cursor = 0;
            MN.cursor2 = 0;
            MN.scroll = 0;
            MN.scroll2 = 0;
            MN.last = -1;
            SE(0x7D5);
            FldMenuShopListBuild();
        } else if (n >= 2 && (padPort0[0].trigger & 4)) {
            if (MN.sel != 0) {
                MN.sel--;
            } else {
                MN.sel = n - 1;
            }
            FldMenuWinClose(MN.buf + 8, 0, 0);
            MN.buf ^= 1;
            FldMenuWinOpen(MN.buf + 8, 0x184, 0xCC, 0x1B8, 0xBE, 0, 1, 0x48010);
            FldMenuWinSetActive(MN.buf + 8);
            MenuLrArrowsFlash(1);
            MN.cursor = 0;
            MN.cursor2 = 0;
            MN.scroll = 0;
            MN.scroll2 = 0;
            MN.last = -1;
            FldMenuShopListBuild();
            SE(0x7D5);
        } else if (padPort0[0].repeat & 0x80) {
            if (MN.mode == 3) {
                switch (MN.tab) {
                case 0:
                    if (W->shopChrA[MN.cursor].name != 0) {
                        ItemCharUnequipEgg(MN.chr, W->shopChrA[MN.cursor].unk2);
                        SE(0x7D2);
                    } else {
                        SE(0x7D4);
                    }
                    break;
                case 1:
                    if (W->shopChrB[MN.cursor].name != 0) {
                        ItemCharUnequipBook(MN.chr, W->shopChrB[MN.cursor].unk2);
                        SE(0x7D2);
                    } else {
                        SE(0x7D4);
                    }
                    break;
                case 2:
                    if (ItemCharUnequipSkill(MN.chr, W->shopChrB[W->u76F8.recs[MN.cursor + MN.scroll].owner].unk2,
                                      W->u76F8.recs[MN.cursor + MN.scroll].unk8) == 0) {
                        SE(0x7D2);
                    } else {
                        SE(0x7D4);
                    }
                    break;
                }
            }
        }
        break;
    }
    case 1: {
        u8 cnt[4][4] = { { 1, 8, 8, 5 }, { 8, 8, 5, 1 }, { 1, 2, 0, 0 }, { 1, 1, 5, 0 } };

        if (MN.state == 2 && MN.mode == 3) {
            n = 3;
        } else if (MN.mode == 3 && MN.tab == 2) {
            FldMenuShopListBuild();
            n = W->unk6EE2;
        } else {
            n = cnt[MN.mode][MN.tab];
        }
        if (MN.sel >= n) {
            MN.sel = 0;
        }
        FldMenuWinTextClear(0xD);
        p = FldMenuWinGetQuadBuf(0xD);
        k = MN.scroll + MN.cursor;
        FldMenuWinTextAdd(0xD, b, W->shop[k].name, 8, 8, 0x14, 0x14, n13++, -1, 8);
        p = FldMenuBarBuild(p, 8, 0x18, 0x12C, 0xC, W->shop[k].type);
        FldMenuWinTextAdd(0xD, b, W->shop[k].name, 8, 8, 0x14, 0x14, n13++, -1, 8);
        s5 = FldMenuWinGetSprBuf(0xD, 1);
        FldMenuWinSetLayer(0xD, 1, &fldMenuTextures[0], s5, 128.0f);
        s5 = FldMenuTaggedNumberBuild(s5, W->shop[k].count, 0, 0xE6, 8, 1.0f, 1.0f, 0, 0);
        FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x30, 0x28, 0x14, 0x14, n13++, 0x39, 2);
        FldMenuBarBuild(p, 0x28, 0x38, 0xA8, 0xC, 0xA);
        if (MN.tab != 0) {
            FldMenuNumberBuild(s5, W->shop[k].price, 0, 0x70, 0x28, 1.0f, 1.0f, 0);
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x60, 0x50, 0x14, 0x14, n13++, 0x3B, 1);
        } else {
            FldMenuNumberBuild(s5, W->shop[k].price * W->shop[k].count, 0, 0x70, 0x28, 1.0f, 1.0f, 0);
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x60, 0x50, 0x14, 0x14, n13++, 0x3A, 1);
        }
        if (MN.cursor2 == 0) {
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x70, 0x14, 0x14, n13++, 0x36, 8);
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x84, 0x14, 0x14, n13++, 0x37, 0);
        } else {
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x70, 0x14, 0x14, n13++, 0x36, 0);
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x84, 0x14, 0x14, n13++, 0x37, 8);
        }
        if (padPort0[0].trigger & 0x20) {
            if (MN.cursor2 == 0) {
            if (MN.tab != 0) {
                if (gold >= W->shop[k].price && ItemCanAdd(W->shop[k].id) != -1) {
                    PtySetGold(gold - W->shop[k].price);
                    ItemAdd(W->shop[k].id);
                    SE(0x7D9);
                    FldMenuWinClose(0xD, 0, 0);
                    MN.state = 0;
                    FldMenuShopListBuild();
                    FldMenuWinSetActive(MN.buf + 8);
                } else {
                    SE(0x7D4);
                }
            } else {
                PtySetGold(gold + W->shop[k].price * W->shop[k].count);
                ItemSetSold(W->shop[k].id);
                ItemBagRemove(W->shop[k].unkA, W->shop[k].count);
                SE(0x7D9);
                FldMenuWinClose(0xD, 0, 0);
                MN.state = 0;
                FldMenuShopListBuild();
                FldMenuWinSetActive(MN.buf + 8);
            }
            } else {
                FldMenuWinClose(0xD, 0, 0);
                MN.state = 0;
                FldMenuShopListBuild();
                FldMenuWinSetActive(MN.buf + 8);
                SE(0x7D3);
            }
        } else if (padPort0[0].trigger & 0x40) {
            FldMenuWinClose(0xD, 0, 0);
            MN.state = 0;
            FldMenuShopListBuild();
            FldMenuWinSetActive(MN.buf + 8);
            SE(0x7D3);
        } else if ((padPort0[0].trigger & 0x1000) && MN.cursor2 != 0) {
            MN.cursor2 = 0;
            SE(0x7D1);
        } else if (padPort0[0].trigger & 0x4000) {
            if (MN.cursor2 == 0) {
                MN.cursor2 = 1;
                SE(0x7D1);
            }
        }
        break;
    }
    case 5: {
        u8 cnt[4][4] = { { 1, 8, 8, 5 }, { 8, 8, 5, 1 }, { 1, 2, 0, 0 }, { 1, 1, 5, 0 } };
        s32 price;

        n = cnt[MN.mode][MN.tab];
        if (MN.sel >= n) {
            MN.sel = 0;
        }
        FldMenuWinTextClear(0xD);
        p = FldMenuWinGetQuadBuf(0xD);
        k = MN.scroll + MN.cursor;
        s5 = FldMenuWinGetSprBuf(0xD, 1);
        FldMenuWinSetLayer(0xD, 1, &fldMenuTextures[0], s5, 128.0f);
        FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x30, 0x28, 0x14, 0x14, n13++, 0x44, 2);
        price = 0;
        p = FldMenuBarBuild(p, 0x28, 0x38, 0xC8, 0xC, 0xA);
        switch (MN.tab) {
        case 0:
            price = W->shop[k].price;
            FldMenuWinTextAdd(0xD, b, W->shop[k].name, 8, 8, 0x14, 0x14, n13++, -1, 8);
            FldMenuBarBuild(p, 8, 0x18, 0x12C, 0xC, W->shop[k].type);
            break;
        case 1:
            if (MN.sel == 0) {
                price = W->books2[k].price;
                FldMenuWinTextAdd(0xD, b, W->books2[k].name, 8, 8, 0x14, 0x14, n13++, -1, 8);
                FldMenuBarBuild(p, 8, 0x18, 0x12C, 0xC, W->books2[k].type + 0xB);
            } else {
                price = W->books[k].price;
                FldMenuWinTextAdd(0xD, b, W->books[k].name, 8, 8, 0x14, 0x14, n13++, -1, 8);
                FldMenuBarBuild(p, 8, 0x18, 0x12C, 0xC, W->books[k].type + 0xB);
            }
            break;
        }
        FldMenuNumberBuild(s5, price, 0, 0x90, 0x28, 1.0f, 1.0f, 0);
        if (MN.tab == 0) {
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0xC, 0x50, 0x14, 0x14, n13++, 0x4A, 1);
        } else {
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x60, 0x50, 0x14, 0x14, n13++, 0x3A, 1);
        }
        if (MN.cursor2 == 0) {
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x70, 0x14, 0x14, n13++, 0x36, 8);
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x84, 0x14, 0x14, n13++, 0x37, 0);
        } else {
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x70, 0x14, 0x14, n13++, 0x36, 0);
            FldMenuWinTextAdd(0xD, b, FONT->tbl[15], 0x76, 0x84, 0x14, 0x14, n13++, 0x37, 8);
        }
        if (padPort0[0].trigger & 0x20) {
            if (MN.cursor2 == 0) {
            if (MN.tab == 0) {
                if (gold >= W->shop[k].price && ItemCanAdd(W->shop[k].id) != -1) {
                    PtySetShopPoints(gold - W->shop[k].price);
                    ItemAdd(W->shop[k].id);
                    SE(0x7D9);
                    FldMenuWinClose(0xD, 0, 0);
                    MN.state = 0;
                    FldMenuShopListBuild();
                    FldMenuWinSetActive(MN.buf + 8);
                } else {
                    SE(0x7D4);
                }
            } else {
                ok = 0;
                if (MN.sel == 0) {
                    if (ItemSkillCheckDiscard(W->books2[k].id) == 0) {
                        ok = 1;
                        PtySetShopPoints(gold + W->books2[k].price);
                        ItemSkillDiscard(W->books2[k].id);
                    }
                } else {
                    if (ItemBookCheckDiscard(W->books[k].unkE) == 0) {
                        ok = 1;
                        PtySetShopPoints(gold + W->books[k].price);
                        ItemBookDiscard(W->books[k].unkE);
                    }
                }
                if (ok != 0) {
                    SE(0x7D9);
                    FldMenuWinClose(0xD, 0, 0);
                    MN.state = 0;
                    FldMenuShopListBuild();
                    FldMenuWinSetActive(MN.buf + 8);
                }
            }
            } else {
                FldMenuWinClose(0xD, 0, 0);
                MN.state = 0;
                FldMenuShopListBuild();
                FldMenuWinSetActive(MN.buf + 8);
                SE(0x7D3);
            }
        } else if (padPort0[0].trigger & 0x40) {
            FldMenuWinClose(0xD, 0, 0);
            MN.state = 0;
            FldMenuShopListBuild();
            FldMenuWinSetActive(MN.buf + 8);
            SE(0x7D3);
        } else if ((padPort0[0].trigger & 0x1000) && MN.cursor2 != 0) {
            MN.cursor2 = 0;
            SE(0x7D1);
        } else if (padPort0[0].trigger & 0x4000) {
            if (MN.cursor2 == 0) {
                MN.cursor2 = 1;
                SE(0x7D1);
            }
        }
        break;
    }
    case 4: {
        u8 cnt[4] = { 3, 3, 1, 2 };

        if (padPort0[0].trigger & 0x1000) {
            if (MN.sel2 != 0) {
                MN.sel2--;
            } else {
                MN.sel2 = cnt[MN.mode];
            }
            SE(0x7D1);
        } else if (padPort0[0].trigger & 0x4000) {
            if (MN.sel2 < cnt[MN.mode]) {
                MN.sel2++;
            } else {
                MN.sel2 = 0;
            }
            SE(0x7D1);
        } else if (padPort0[0].trigger & 0x20) {
            if (MN.mode == 3) {
                MN.state = 2;
                FldMenuGuideSet(0x11);
            } else {
                FldMenuWinSetActive(MN.buf + 8);
                MN.state = 0;
                switch (MN.mode) {
                case 0:
                    if (MN.sel2 == 3) {
                        FldMenuGuideSet(0xD);
                    } else {
                        FldMenuGuideSet(0xC);
                    }
                    break;
                case 1:
                    if (MN.sel2 == 3) {
                        FldMenuGuideSet(0xE);
                    } else {
                        FldMenuGuideSet(0xF);
                    }
                    break;
                case 2:
                    if (MN.sel2 != 0) {
                        FldMenuGuideSet(0xD);
                    } else {
                        FldMenuGuideSet(0x12);
                    }
                    break;
                }
            }
            if (MN.mode == 0) {
                u8 map[4] = { 1, 2, 3, 0 };

                MN.tab = map[MN.sel2];
            } else {
                MN.tab = MN.sel2;
            }
            MN.sel = 0;
            MN.cursor = 0;
            MN.cursor2 = 0;
            MN.scroll = 0;
            MN.scroll2 = 0;
            FldMenuShopListBuild();
            FldMenuWinClose(0xE, 0, 1);
            SE(0x7D2);
        } else if (padPort0[0].trigger & 0x40) {
            FldMenuClose();
            SE(0x7D2);
        }
        break;
    }
    case 2: {
        u8 cnt[4][4] = { { 1, 8, 8, 5 }, { 8, 8, 5, 1 }, { 1, 2, 0, 0 }, { 1, 1, 5, 0 } };

        if (MN.state == 2 && MN.mode == 3) {
            n = 3;
        } else if (MN.mode == 3 && MN.tab == 2) {
            FldMenuShopListBuild();
            n = W->unk6EE2;
        } else {
            n = cnt[MN.mode][MN.tab];
        }
        if (MN.sel >= n) {
            MN.sel = 0;
        }
        if (padPort0[0].trigger & 0x20) {
            FldMenuGuideSet(0x10);
            MN.state = 0;
            MN.cursor = 0;
            MN.cursor2 = 0;
            FldMenuShopListBuild();
            FldMenuWinSetActive(MN.buf + 8);
            if (MN.scale[MN.chr] == 3.0f) {
                MN.scale[MN.chr] = 1.0f;
                MN.vel[MN.chr] = 0.15f;
            }
            SE(0x803);
        } else if (padPort0[0].repeat & 0x80) {
            if (MN.mode == 3) {
                r = 0;
                switch (MN.tab) {
                case 0:
                    r = ItemCharUnequipAllEggs(MN.chr);
                    break;
                case 1:
                    r = ItemCharUnequipAllBooks(MN.chr);
                    break;
                case 2:
                    r = ItemCharUnequipAllSkills(MN.chr);
                    break;
                }
                if (r != 0) {
                    SE(0x7D2);
                } else {
                    SE(0x7D4);
                }
            }
        } else if (MN.mode == 3 && (padPort0[0].trigger & 4)) {
            MenuLrArrowsFlash(1);
            if (MN.tab != 0) {
                MN.tab--;
            } else {
                MN.tab = 2;
            }
            SE(0x7D5);
            for (i = 0; i < 6; i++) {
                if (W->shopChrB[i].name != 0) {
                    MN.sel = i;
                    break;
                }
            }
        } else if (MN.mode == 3 && (padPort0[0].trigger & 8)) {
            MenuLrArrowsFlash(0);
            if (MN.tab < 2) {
                MN.tab++;
            } else {
                MN.tab = 0;
            }
            SE(0x7D5);
            if (MN.tab == 2) {
                for (i = 0; i < 6; i++) {
                    if (W->shopChrB[i].name != 0) {
                        MN.sel = i;
                        break;
                    }
                }
            }
        } else if (padPort0[0].trigger & 0x40) {
            MN.state = 4;
            FldMenuShopListBuild();
            switch (MN.mode) {
            case 0:
                FldMenuWinOpen(0xE, 0x140, 0xE0, 0xC8, 0x80, 0, 2, 0x80F0);
                break;
            case 1:
                FldMenuWinOpen(0xE, 0x154, 0xE0, 0xF0, 0x80, 0, 2, 0x80F0);
                break;
            case 2:
                FldMenuWinOpen(0xE, 0x14A, 0xF0, 0x104, 0x50, 0, 2, 0x80F0);
                break;
            case 3:
                FldMenuClose();
                SE(0x7D2);
                return 0xFF;
            }
            FldMenuWinSetActive(0xE);
            SE(0x7D3);
        } else if (padPort0[0].trigger & 0x1000) {
            if (MN.chr != 0) {
                MN.chr--;
            } else {
                MN.chr = nChr - 1;
            }
            FldMenuShopListBuild();
            SE(0x7D1);
        } else if (padPort0[0].trigger & 0x4000) {
            if (MN.chr < nChr - 1) {
                MN.chr++;
            } else {
                MN.chr = 0;
            }
            FldMenuShopListBuild();
            SE(0x7D1);
        }
    end:
        break;
    }
    case 3: {
        u8 cnt[4][4] = { { 1, 8, 8, 5 }, { 8, 8, 5, 1 }, { 1, 2, 0, 0 }, { 1, 1, 5, 0 } };
        s32 rows3;
        s32 size3;
        s32 color3;

        p = FldMenuWinGetQuadBuf(0xD);
        FldMenuWinTextClear(0xD);
        if (MN.state == 2 && MN.mode == 3) {
            n = 3;
        } else if (MN.mode == 3 && MN.tab == 2) {
            FldMenuShopListBuild();
            n = W->unk6EE2;
        } else {
            n = cnt[MN.mode][MN.tab];
        }
        if (MN.sel >= n) {
            MN.sel = 0;
        }
        s5 = FldMenuWinGetSprBuf(0xD, 3);
        FldMenuWinSetLayer(0xD, 3, &fldMenuTextures[1], s5, 128.0f);
        name = 0;
        s2 = FldMenuWinGetSprBuf(0xD, 2);
        sub = 0;
        FldMenuWinSetLayer(0xD, 2, &fldMenuTextures[0], s2, 128.0f);
        icon = 0;
        s3 = FldMenuWinGetSprBuf(0xD, 4);
        m = 0;
        FldMenuWinSetLayer(0xD, 4, &fldMenuTextures[4], s3, 128.0f);
        flag = -1;
        s4 = FldMenuWinGetSprBuf(0xD, 1);
        FldMenuWinSetLayer(0xD, 1, &fldMenuTextures[2], s4, 128.0f);
        switch (MN.tab) {
        case 0:
            m = W->unk6ED4;
            break;
        case 1:
            m = W->books3Count;
            break;
        case 2:
            m = W->books4Count;
            break;
        }
        FldMenuListFixOnRemove(&MN.cursor2, &MN.scroll2, 7, 3, m);
        if (padPort0[0].repeatFast & 0x1000) {
            FldMenuListCursorUp(&MN.cursor2, &MN.scroll2, 7, 3, m);
        }
        if (padPort0[0].repeatFast & 0x4000) {
            FldMenuListCursorDown(&MN.cursor2, &MN.scroll2, 7, 3, m);
        }
        k = MN.scroll2;
        lt8 = m < 8;
if (k != 0) { lt = 1; } else { lt = 0; }
if (!(lt8)) { if (k + 7 < m) { rt = 1; } else { rt = 0; } } else { rt = 0; }
        rows3 = 7;
        FldMenuWinSetScrollArrows(0xD, lt, rt);
        if (lt8) {
            rows3 = m;
        }
        FldMenuCursorSetAt(0xD, (f32)((rows3 - MN.cursor2) * 6) + 16.0f, (f32)(MN.cursor2 * 24) + 24.0f);
        for (i = 0; i < rows3; i++, k++) {
            if (i == MN.cursor2) {
                MN.ofs2[k] -= 8;
                color3 = 8;
                if (MN.ofs2[k] < (rows3 - i) * 6) {
                    MN.ofs2[k] = (rows3 - i) * 6;
                }
                size3 = 0x16;
            } else {
                MN.ofs2[k] += 8;
                color3 = 0;
                if (MN.ofs2[k] >= (rows3 - i) * 6 + 0x10) {
                    MN.ofs2[k] = (rows3 - i) * 6 + 0x10;
                }
                size3 = 0x14;
            }
            switch (MN.tab) {
            case 0:
                name = W->magic[k].name;
                sub = W->magic[k].unk5;
                icon = W->magic[k].icon;
                if (name != 0) {
                    s4->type = 1;
                    s4->v[0].r = 0x80;
                    s4->v[0].g = 0x80;
                    s4->v[0].b = 0x80;
                    s4->v[0].a = 0x80;
                    s4->v[0].x = MN.ofs2[k] + 0xE6;
                    s4->v[0].y = i * 24 + 0x12;
                    s4->v[1].x = MN.ofs2[k] + 0x104;
                    s4->v[1].y = i * 24 + 0x26;
                    s4->v[0].u = 0xCE;
                    s4->v[0].v = 0x6E;
                    s4->v[1].u = 0xEC;
                    s4->v[1].v = 0x82;
                    s4->z = 0;
                    s4++;
                    s2 = FldMenuHpFractionBuild(s2, W->magic[k].unkA, W->magic[k].unk8, MN.ofs2[k] + 0x10A, i * 24 + 0x14);
                }
                for (j = 0; j < W->magic[k].n; j++) {
                    VuSinCosDeg(&sn, &cs, (f32)j * 110.0f + (f32)MN.ang2);
                    s3->type = 1;
                    if (W->magic[k].n == 1) {
                        s3->v[0].r = 0x80;
                        s3->v[0].g = 0x80;
                        s3->v[0].b = 0x80;
                        s3->v[0].a = 0;
                    } else {
                        s3->v[0].r = 0xA0;
                        s3->v[0].g = 0x80;
                        s3->v[0].b = 0x80;
                        s3->v[0].a = 0;
                    }
                    s3->v[0].x = MN.ofs2[k] + (s32)(cs * 7.0f);
                    s3->v[0].y = i * 24 + 0x10 + (s32)(sn * 5.0f);
                    s3->v[1].x = MN.ofs2[k] + (s32)(cs * 7.0f) + 0xA;
                    s3->v[1].y = i * 24 + ((s32)(sn * 5.0f) + 0x1A);
                    s3->v[0].u = sprStarIconRects[0].u0;
                    s3->v[0].v = sprStarIconRects[0].v0;
                    s3->v[1].u = sprStarIconRects[0].u1;
                    s3->v[1].v = sprStarIconRects[0].v1;
                    s3->z = 0;
                    s3++;
                }
                break;
            case 1:
                for (j = 0; j < W->books3[k].n; j++) {
                    s5->type = 1;
                    s5->v[0].r = 0x80;
                    s5->v[0].g = 0x80;
                    s5->v[0].b = 0x80;
                    s5->v[0].a = 0;
                    s5->v[0].x = MN.ofs2[k] + 0x11C + j * 20;
                    s5->v[0].y = i * 24 + 0x10;
                    s5->v[1].x = MN.ofs2[k] + menuItemIcons[0x4E].w + 0x11C + j * 20;
                    s5->v[1].y = i * 24 + (menuItemIcons[0x4E].h + 0x10);
                    s5->v[0].u = menuItemIcons[0x4E].u0;
                    s5->v[0].v = menuItemIcons[0x4E].v0;
                    s5->v[1].u = menuItemIcons[0x4E].u1;
                    s5->v[1].v = menuItemIcons[0x4E].v1;
                    s5->z = 0;
                    s5++;
                }
                name = W->books3[k].name;
                sub = W->books3[k].type + 0xB;
                icon = W->books3[k].icon;
                if (name != 0) {
                    FldMenuWinTextAddIndexed(0xD, b, FONT->tbl[10], MN.ofs2[k] + 0x104, i * 24 + 0x10, 0x14, 0x14,
                                  W->books3[k].type, 3);
                }
                break;
            case 2:
                name = W->books4[k].name;
                sub = W->books4[k].type + 0xB;
                icon = W->books4[k].icon;
                flag = W->books4[k].icon;
                break;
            }
            if (name != 0) {
                FldMenuWinTextAdd(0xD, b, name, MN.ofs2[k] + 0x1C, i * 24 + 0x10, size3, size3, n13++, -1, color3);
                if (flag >= 0) {
                    FldMenuWinTextAddIndexed(0xD, b, FONT->tbl[10], MN.ofs2[k] + 0xDE, i * 24 + 0x12, 0x14, 0x14,
                                  W->books4[k].type, 3);
                    if (MN.timer > 0x80) {
                        for (j = 0; j < W->books4[k].n; j++) {
                            s3->type = 1;
                            s3->v[0].r = 0x80;
                            s3->v[0].g = 0x80;
                            s3->v[0].b = 0x80;
                            s3->v[0].a = 0;
                            s3->v[0].x = j * 16 + 0xF6 + MN.ofs2[k];
                            s3->v[0].y = i * 24 + 0x12;
                            s3->v[1].x = MN.bar[j] + j * 16 + 0xF6 + MN.ofs2[k];
                            s3->v[1].y = i * 24 + (MN.bar[j] + 0x12);
                            s3->v[0].u = sprStarIconRects[0].u0;
                            s3->v[0].v = sprStarIconRects[0].v0;
                            s3->v[1].u = sprStarIconRects[0].u1;
                            s3->v[1].v = sprStarIconRects[0].v1;
                            s3->z = 0;
                            s3++;
                        }
                        if (W->books4[k].n == W->books4[k].max) {
                            s3->type = 1;
                            s3->v[0].r = 0x80;
                            s3->v[0].g = 0x80;
                            s3->v[0].b = 0x80;
                            s3->v[0].a = 0;
                            s3->v[0].x = MN.ofs2[k] + 0xFA;
                            s3->v[0].y = i * 24 + 0xE;
                            s3->v[1].x = MN.ofs2[k] + ((u16)sprStarIconRects[1].w + 0xFA);
                            s3->v[1].y = i * 24 + ((u16)sprStarIconRects[1].h + 0xE);
                            s3->v[0].u = sprStarIconRects[1].u0;
                            s3->v[0].v = sprStarIconRects[1].v0;
                            s3->v[1].u = sprStarIconRects[1].u1;
                            s3->v[1].v = sprStarIconRects[1].v1;
                            s3->z = 0;
                            s3++;
                        }
                    } else {
                        s2 = FldMenuFractionBuild(s2, W->books4[k].unkC, W->books4[k].unkE, MN.ofs2[k] + 0xF6, i * 24 + 0x15);
                    }
                    s3->type = 1;
                    s3->v[0].r = 0x80;
                    s3->v[0].g = 0x80;
                    s3->v[0].b = 0x80;
                    s3->v[0].a = 0;
                    s3->v[0].x = MN.ofs2[k];
                    s3->v[0].y = i * 24 + 0xE;
                    s3->v[1].x = MN.ofs2[k] + (u16)menuSkillTypeIcons[icon].w;
                    s3->v[1].y = i * 24 + ((u16)menuSkillTypeIcons[icon].h + 0xE);
                    s3->v[0].u = menuSkillTypeIcons[icon].u0;
                    s3->v[0].v = menuSkillTypeIcons[icon].v0;
                    s3->v[1].u = menuSkillTypeIcons[icon].u1;
                    s3->v[1].v = menuSkillTypeIcons[icon].v1;
                    s3->z = 0;
                    s3++;
                } else {
                    s5->type = 1;
                    s5->v[0].r = 0x80;
                    s5->v[0].g = 0x80;
                    s5->v[0].b = 0x80;
                    s5->v[0].a = 0;
                    s5->v[0].x = MN.ofs2[k];
                    s5->v[0].y = i * 24 + 0x10;
                    s5->v[1].x = MN.ofs2[k] + menuItemIcons[icon].w;
                    s5->v[1].y = i * 24 + (menuItemIcons[icon].h + 0x10);
                    s5->v[0].u = menuItemIcons[icon].u0;
                    s5->v[0].v = menuItemIcons[icon].v0;
                    s5->v[1].u = menuItemIcons[icon].u1;
                    s5->v[1].v = menuItemIcons[icon].v1;
                    s5->z = 0;
                    s5++;
                }
            }
            p = FldMenuBarBuild(p, MN.ofs2[k], i * 24 + 0x20, 0x168, 0xC, sub);
        }
        p->type = 0;
        s5->type = 0;
        s2->type = 0;
        s3->type = 0;
        s4->type = 0;
        if (padPort0[0].trigger & 0x40) {
            FldMenuWinClose(0xD, 0, 0);
            MN.toggle = 0;
            MN.last = -1;
            MN.state = 0;
            FldMenuShopListBuild();
            FldMenuWinSetActive(MN.buf + 8);
            SE(0x7D3);
        } else if (padPort0[0].repeat & 0x20) {
            switch (MN.tab) {
            case 0:
                ItemCharEquipEgg(MN.chr, W->shopChrA[MN.cursor].unk2, W->magic[MN.scroll2 + MN.cursor2].unk6);
                break;
            case 1:
                ItemCharEquipBook(MN.chr, W->shopChrB[MN.cursor].unk2, W->books3[MN.scroll2 + MN.cursor2].unkE);
                break;
            case 2:
                ItemCharEquipSkill(MN.chr, W->shopChrB[W->u76F8.recs[MN.cursor + MN.scroll].owner].unk2,
                              W->u76F8.recs[MN.cursor + MN.scroll].unk8, W->books4[MN.scroll2 + MN.cursor2].id);
                break;
            }
            MN.last = -1;
            MN.toggle = 0;
            FldMenuWinClose(0xD, 0, 0);
            MN.state = 0;
            FldMenuShopListBuild();
            FldMenuWinSetActive(MN.buf + 8);
            SE(0x7D2);
        }
        break;
    }
    }
    {
    s32 size;
    s32 color;
    s32 rows;
    s32 price;
    s32 jt;
    {
        u8 cnt[4][4] = { { 1, 8, 8, 5 }, { 8, 8, 5, 1 }, { 1, 2, 0, 0 }, { 1, 1, 5, 0 } };

        if (MN.state == 2 && MN.mode == 3) {
            FldMenuShopListBuild();
            n = 3;
        } else if (MN.mode == 3 && MN.tab == 2) {
            FldMenuShopListBuild();
            n = W->unk6EE2;
        } else {
            n = cnt[MN.mode][MN.tab];
        }
        if (MN.sel >= n) {
            MN.sel = 0;
        }
    }
    {
        if (n >= 2 && MN.state == 0) {
            if (MN.mode == 3 && MN.tab == 2) {
                FldMenuWinLrArrowsSet(0xC, 0, -12.0f);
            } else {
                FldMenuWinLrArrowsSet(0xC, 1, -12.0f);
            }
        } else if (MN.state == 2 && MN.mode == 3) {
            FldMenuWinLrArrowsSet(0xC, 1, -12.0f);
        } else {
            FldMenuWinLrArrowsSet(0xC, 0, -12.0f);
        }
        if (padPort0[0].trigger & 0x10) {
            if (MN.toggle != 0) {
                MN.toggle = 0;
            } else {
                MN.toggle = 1;
            }
            MN.last = -1;
        }
        switch (MN.mode) {
        case 0:
        case 1:
        default:
            m = W->unk110E;
            break;
        case 2:
            if (MN.tab == 0) {
                m = W->unk110E;
            } else if (MN.sel != 0) {
                m = W->counts[2];
            } else {
                m = W->counts[3];
            }
            break;
        case 3:
            switch (MN.tab) {
            case 0:
            default:
                m = W->unk6EDC;
                break;
            case 1:
                m = W->unk6EDE;
                break;
            case 2:
                m = W->unk76F4;
                break;
            }
            break;
        }
        if (m != 0 && MN.state == 0) {
            FldMenuListFixOnRemove(&MN.cursor, &MN.scroll, 7, 3, m);
            if (padPort0[0].repeatFast & 0x1000) {
                FldMenuListCursorUp(&MN.cursor, &MN.scroll, 7, 3, m);
            }
            if (padPort0[0].repeatFast & 0x4000) {
                FldMenuListCursorDown(&MN.cursor, &MN.scroll, 7, 3, m);
            }
        }
        FldMenuWinTextClear(0xB);
        FldMenuWinTextClear(0xA);
        FldMenuWinTextClear(0xC);
        FldMenuWinTextClear(0xE);
        FldMenuWinTextClear(MN.buf + 8);
        if (m != 0) {
            if (MN.scroll + MN.cursor >= m) {
                if (MN.scroll != 0) {
                    if (MN.scroll + 7 >= m) {
                        MN.scroll = m - 1 - MN.cursor;
                    } else {
                        MN.scroll = m - 8;
                    }
                } else {
                    MN.cursor = m - 1;
                }
            }
        }
        k = MN.scroll;
        flag = -1;
        name = 0;
        MN.timer++;
        icon = 0;
        eq6 = 0;
if (k != 0) { lt = 1; } else { lt = 0; }
if (!(m < 8)) { if (k + 7 < m) { rt = 1; } else { rt = 0; } } else { rt = 0; }
        rows = 7;
        FldMenuWinSetScrollArrows(8, lt, rt);
        if (m < rows) {
            rows = m;
        }
        switch (MN.state) {
        case 0:
            FldMenuCursorSetAt(MN.buf + 8, (f32)((rows - MN.cursor) * 6) + 8.0f, (f32)(MN.cursor * 24) + 16.0f);
            break;
        case 1:
        case 5:
            FldMenuCursorSetAt(0xD, 118.0f - (f32)(MN.cursor2 * 10), (f32)(MN.cursor2 * 20) + 120.0f);
            break;
        case 2:
            FldMenuCursorSetAt(0xA, 32.0f, (f32)(MN.chr * 40) + 48.0f);
            break;
        case 4:
            FldMenuCursorSetAt(0xE, 20.0f, (f32)(MN.sel2 * 24) + 30.0f);
            break;
        }
        p = FldMenuWinGetQuadBuf(MN.buf + 8);
        s5 = FldMenuWinGetSprBuf(MN.buf + 8, 3);
        FldMenuWinSetLayer(MN.buf + 8, 3, &fldMenuTextures[1], s5, 128.0f);
        s2 = FldMenuWinGetSprBuf(MN.buf + 8, 2);
        FldMenuWinSetLayer(MN.buf + 8, 2, &fldMenuTextures[0], s2, 128.0f);
        s3 = FldMenuWinGetSprBuf(MN.buf + 8, 4);
        FldMenuWinSetLayer(MN.buf + 8, 4, &fldMenuTextures[4], s3, 128.0f);
        s4 = FldMenuWinGetSprBuf(MN.buf + 8, 1);
        FldMenuWinSetLayer(MN.buf + 8, 1, &fldMenuTextures[2], s4, 128.0f);
        MN.help = 0;
        {
            u8 *fp = &MN.flags[7];

            for (jt = 7; jt >= 0; jt--) {
                *fp-- = 0;
            }
        }
        if (rows == 0) {
            switch (MN.mode) {
            case 0: {
                u8 t[4] = { 0x4F, 0x4E, 0x4E, 0x4E };

                w = TxtMsgGetWidth(FONT->tbl[15], t[MN.tab], 0x14);
                FldMenuWinTextAdd(MN.buf + 8, MN.buf, FONT->tbl[15], 0xE0 - w / 2, 0x56, 0x14, 0x14, n8++, t[MN.tab], 3);
                break;
            }
            case 1: {
                u8 t[4] = { 0x50, 0x50, 0x50, 0x4F };

                w = TxtMsgGetWidth(FONT->tbl[15], t[MN.tab], 0x14);
                FldMenuWinTextAdd(MN.buf + 8, MN.buf, FONT->tbl[15], 0xE0 - w / 2, 0x56, 0x14, 0x14, n8++, t[MN.tab], 3);
                break;
            }
            case 2: {
                u8 t[2] = { 0x4D, 0x4C };

                if (MN.tab != 0) {
                    w = TxtMsgGetWidth(FONT->tbl[15], t[MN.tab], 0x14);
                    FldMenuWinTextAdd(MN.buf + 8, MN.buf, FONT->tbl[15], 0xE0 - w / 2, 0x56, 0x14, 0x14, n8++, t[MN.sel], 3);
                } else {
                    w = TxtMsgGetWidth(FONT->tbl[15], 0x4E, 0x14);
                    FldMenuWinTextAdd(MN.buf + 8, MN.buf, FONT->tbl[15], 0xE0 - w / 2, 0x56, 0x14, 0x14, n8++, 0x4E, 3);
                }
                break;
            }
            case 3:
                w = TxtMsgGetWidth(FONT->tbl[15], 0x4B, 0x14);
                FldMenuWinTextAdd(MN.buf + 8, MN.buf, FONT->tbl[15], 0xE0 - w / 2, 0x56, 0x14, 0x14, n8++, 0x4B, 3);
                break;
            }
        }
        for (i = 0; i < rows; i++, k++) {
            switch (MN.mode) {
            case 0:
            default:
                price = W->shop[k].price;
                name = W->shop[k].name;
                sub = W->shop[k].type;
                icon = W->shop[k].icon;
                if (MN.tab != 0 && W->shop[k].unk9 != 0 && MN.unk227 >= 10) {
                    s3->type = 1;
                    s3->v[0].r = 0x80;
                    s3->v[0].g = 0x80;
                    s3->v[0].b = 0x80;
                    s3->v[0].a = 0;
                    s3->v[0].x = MN.ofs[k] + 0xEC;
                    s3->v[0].y = i * 24 + 8;
                    s3->v[1].x = MN.ofs[k] + 0x11A;
                    s3->v[1].y = i * 24 + 0x1C;
                    s3->v[0].u = 0xD2;
                    s3->v[0].v = 0xAA;
                    s3->v[1].u = 0x100;
                    s3->v[1].v = 0xBE;
                    s3->z = 0;
                    s3++;
                }
                break;
            case 1:
                name = W->shop[k].name;
                sub = W->shop[k].type;
                icon = W->shop[k].icon;
                price = W->shop[k].count;
                break;
            case 2:
                if (MN.tab == 0) {
                    name = W->shop[k].name;
                    sub = W->shop[k].type;
                    icon = W->shop[k].icon;
                    price = W->shop[k].price;
                } else if (MN.sel != 0) {
                    name = W->books[k].name;
                    sub = W->books[k].type + 0xB;
                    icon = W->books[k].icon;
                    price = W->books[k].price;
                    FldMenuWinTextAddIndexed(MN.buf + 8, MN.buf, FONT->tbl[10], MN.ofs[k] + 0xE8, i * 24 + 0xC, 0x14, 0x14,
                                  W->books[k].type, 3);
                } else {
                    flag = 1;
                    name = W->books2[k].name;
                    sub = W->books2[k].type + 0xB;
                    price = W->books2[k].price;
                    icon = W->books2[k].icon;
                }
                break;
            case 3:
                switch (MN.tab) {
                case 0:
                    sub = 1;
                    name = W->shopChrA[k].name;
                    icon = W->shopChrA[k].icon;
                    if (name != 0) {
                        s4->type = 1;
                        s4->v[0].r = 0x80;
                        s4->v[0].g = 0x80;
                        s4->v[0].b = 0x80;
                        s4->v[0].a = 0x80;
                        s4->v[0].x = MN.ofs[k] + 0xF0;
                        s4->v[0].y = i * 24 + 0xC;
                        s4->v[1].x = MN.ofs[k] + 0x10E;
                        s4->v[1].y = i * 24 + 0x20;
                        s4->v[0].u = 0xCE;
                        s4->v[0].v = 0x6E;
                        s4->v[1].u = 0xEC;
                        s4->v[1].v = 0x82;
                        s4->z = 0;
                        s4++;
                        s2 = FldMenuHpFractionBuild(s2, W->shopChrA[k].unk4, W->shopChrA[k].unk6, MN.ofs[k] + 0x114, i * 24 + 0xE);
                        for (jt = 0; jt < W->shopChrA[k].unk8; jt++) {
                            VuSinCosDeg(&sn2, &cs2, (f32)jt * 110.0f + (f32)MN.ang2);
                            s3->type = 1;
                            if (W->shopChrA[k].unk8 == 1) {
                                s3->v[0].r = 0x80;
                                s3->v[0].g = 0x80;
                                s3->v[0].b = 0x80;
                                s3->v[0].a = 0;
                            } else {
                                s3->v[0].r = 0xA0;
                                s3->v[0].g = 0x80;
                                s3->v[0].b = 0x80;
                                s3->v[0].a = 0;
                            }
                            s3->v[0].x = MN.ofs[k] + 4 + (s32)(cs2 * 7.0f);
                            s3->v[0].y = i * 24 + 8 + (s32)(sn2 * 5.0f);
                            s3->v[1].x = (s16)(MN.ofs[k] + 0xE) + (s32)(cs2 * 7.0f);
                            s3->v[1].y = i * 24 + ((s32)(sn2 * 5.0f) + 0x12);
                            s3->v[0].u = sprStarIconRects[0].u0;
                            s3->v[0].v = sprStarIconRects[0].v0;
                            s3->v[1].u = sprStarIconRects[0].u1;
                            s3->v[1].v = sprStarIconRects[0].v1;
                            s3->z = 0;
                            s3++;
                        }
                    }
                    break;
                case 1:
                    name = W->shopChrB[k].name;
                    sub = W->shopChrB[k].unkA + 0xB;
                    if (name != 0) {
                        FldMenuWinTextAddIndexed(MN.buf + 8, MN.buf, FONT->tbl[10], MN.ofs[k] + 0x104, i * 24 + 8, 0x14, 0x14,
                                      W->shopChrB[k].unkA, 3);
                        for (jt = 0; jt < W->shopChrB[k].unkC; jt++) {
                            if (W->shopChrB[k].slots[jt].unk4 != 0) {
                                icon = W->shopChrB[k].slots[jt].unk2;
                                s3->type = 1;
                                s3->v[0].r = 0x80;
                                s3->v[0].g = 0x80;
                                s3->v[0].b = 0x80;
                                s3->v[0].a = 0;
                                s3->v[0].x = jt * 24 + 0x11C + MN.ofs[k];
                                s3->v[0].y = i * 24 + 8;
                                s3->v[1].x = (u16)menuSkillTypeIcons[icon].w + jt * 24 + 0x11C + MN.ofs[k];
                                s3->v[1].y = i * 24 + ((u16)menuSkillTypeIcons[icon].h + 8);
                                s3->v[0].u = menuSkillTypeIcons[icon].u0;
                                s3->v[0].v = menuSkillTypeIcons[icon].v0;
                                s3->v[1].u = menuSkillTypeIcons[icon].u1;
                                s3->v[1].v = menuSkillTypeIcons[icon].v1;
                                s3->z = 0;
                                s3++;
                            } else {
                                s5->type = 1;
                                s5->v[0].r = 0x80;
                                s5->v[0].g = 0x80;
                                s5->v[0].b = 0x80;
                                s5->v[0].a = 0;
                                s5->v[0].x = jt * 24 + 0x11C + MN.ofs[k];
                                s5->v[0].y = i * 24 + 8;
                                s5->v[1].x = menuItemIcons[0x4E].w + jt * 24 + 0x11C + MN.ofs[k];
                                s5->v[1].y = i * 24 + (menuItemIcons[0x4E].h + 8);
                                s5->v[0].u = menuItemIcons[0x4E].u0;
                                s5->v[0].v = menuItemIcons[0x4E].v0;
                                s5->v[1].u = menuItemIcons[0x4E].u1;
                                s5->v[1].v = menuItemIcons[0x4E].v1;
                                s5->z = 0;
                                s5++;
                            }
                        }
                    }
                    icon = W->shopChrB[k].icon;
                    break;
                case 2:
                    flag = 1;
                    name = W->u76F8.recs[k].name;
                    sub = W->u76F8.recs[k].type + 0xB;
                    icon = W->u76F8.recs[k].icon;
                    break;
                }
                price = 0;
                break;
            }
            if (i == MN.cursor) {
                MN.ofs[k] -= 8;
                if (MN.ofs[k] < (rows - i) * 6) {
                    MN.ofs[k] = (rows - i) * 6;
                }
                color = 8;
                size = 0x16;
                switch (MN.mode) {
                case 2:
                    if (MN.tab == 0) {
                        for (jt = 0; jt < 8; jt++) {
                            diff[jt] = W->shop[k].stats[jt];
                        }
                        MN.help = W->shop[k].help;
                        MN.helpType = W->shop[k].unk8;
                        if (W->shop[k].unk19 == 6) {
                            eq6 = 1;
                        }
                        for (jt = 0; jt < 8; jt++) {
                            if ((W->shop[k].mask >> jt) & 1) {
                                MN.flags[jt] = 1;
                            }
                        }
                    } else {
                        for (jt = 0; jt < 8; jt++) {
                            diff[jt] = 0;
                            MN.flags[jt] = 0;
                        }
                        if (MN.sel != 0) {
                            MN.help = W->books[k].help;
                            MN.helpType = 3;
                            if (W->books[k].unk3 >= 0) {
                                MN.flags[W->books[k].unk3] = 1;
                            }
                        } else {
                            MN.helpType = 2;
                            MN.help = W->books2[k].help;
                            if (W->books2[k].unk3 >= 0) {
                                MN.flags[W->books2[k].unk3] = 1;
                            }
                        }
                    }
                    break;
                case 0:
                case 1:
                default:
                    for (jt = 0; jt < 8; jt++) {
                        diff[jt] = W->shop[k].stats[jt];
                    }
                    MN.help = W->shop[k].help;
                    MN.helpType = W->shop[k].unk8;
                    if (W->shop[k].unk19 == 6) {
                        eq6 = 1;
                    }
                    for (jt = 0; jt < 8; jt++) {
                        if ((W->shop[k].mask >> jt) & 1) {
                            MN.flags[jt] = 1;
                        }
                    }
                    break;
                case 3:
                    for (jt = 0; jt < 8; jt++) {
                        diff[jt] = 0;
                        MN.flags[jt] = 0;
                    }
                    switch (MN.tab) {
                    case 0:
                        MN.helpType = 3;
                        MN.help = W->shopChrA[k].help;
                        break;
                    case 1:
                        MN.helpType = 3;
                        MN.help = W->shopChrB[k].help;
                        break;
                    case 2:
                        MN.helpType = 2;
                        MN.help = W->u76F8.recs[k].help;
                        break;
                    }
                    break;
                }
            } else {
                MN.ofs[k] += 8;
                if (MN.ofs[k] >= (rows - i) * 6 + 0x10) {
                    MN.ofs[k] = (rows - i) * 6 + 0x10;
                }
                color = 0;
                size = 0x14;
            }
            if (name == 0) {
                if (MN.mode == 3 && MN.tab == 2) {
                    p = FldMenuBarBuild(p, MN.ofs[k], i * 24 + 0x18, 0x172, 0xC,
                                        W->shopChrB[W->u76F8.recs[k].owner].unkA + 0xB);
                    FldMenuWinTextAddIndexed(MN.buf + 8, MN.buf, FONT->tbl[10], MN.ofs[k] + 0x104, i * 24 + 8, 0x14, 0x14,
                                  W->shopChrB[W->u76F8.recs[k].owner].unkA, 3);
                } else {
                    p = FldMenuBarBuild(p, MN.ofs[k], i * 24 + 0x18, 0x172, 0xC, 0);
                }
            } else {
                p = FldMenuBarBuild(p, MN.ofs[k], i * 24 + 0x18, 0x172, 0xC, sub);
            }
            if (name != 0) {
                if ((MN.mode == 0 && MN.tab != 0) || (MN.mode == 2 && MN.tab == 0)) {
                    if (gold < W->shop[k].price) {
                        color = 0xA;
                    }
                }
                FldMenuWinTextAdd(MN.buf + 8, MN.buf, name, MN.ofs[k] + 0x24, i * 24 + 8, 0x14, size, n8++, -1, color);
                switch (MN.mode) {
                case 0:
                    s2 = FldMenuNumberBuild(s2, price, 1, MN.ofs[k] + 0x172, i * 24 + 8, 1.0f, 1.0f, 0);
                    break;
                case 2:
                    if (MN.sel == 0 && MN.tab == 1) {
                        if ((s8)MN.timer >= 0) {
                            s2 = FldMenuNumberBuild(s2, price, 1, MN.ofs[k] + 0x172, i * 24 + 8, 1.0f, 1.0f, 0);
                        }
                    } else {
                        s2 = FldMenuNumberBuild(s2, price, 1, MN.ofs[k] + 0x172, i * 24 + 8, 1.0f, 1.0f, 0);
                    }
                    break;
                case 1:
                    if (sub != MN.mode || MN.tab != 3) {
                        s2 = FldMenuTaggedNumberBuild(s2, price, 0, MN.ofs[k] + 0x12A, i * 24 + 8, 1.0f, 1.0f, 0, 0);
                    }
                    break;
                }
                if (flag >= 0) {
                    if (MN.mode != 2) {
                        FldMenuWinTextAddIndexed(MN.buf + 8, MN.buf, FONT->tbl[10], MN.ofs[k] + 0xE8, i * 24 + 0xC, 0x14, 0x14,
                                      W->u76F8.recs[k].type, 3);
                        if (MN.timer > 0x80) {
                            for (jt = 0; jt < W->u76F8.recs[k].n; jt++) {
                                s3->type = 1;
                                s3->v[0].r = 0x80;
                                s3->v[0].g = 0x80;
                                s3->v[0].b = 0x80;
                                s3->v[0].a = 0;
                                s3->v[0].x = jt * 16 + 0x100 + MN.ofs[k];
                                s3->v[0].y = i * 24 + 0xC;
                                s3->v[1].x = MN.bar[jt] + jt * 16 + 0x100 + MN.ofs[k];
                                s3->v[1].y = i * 24 + (MN.bar[jt] + 0xC);
                                s3->v[0].u = sprStarIconRects[0].u0;
                                s3->v[0].v = sprStarIconRects[0].v0;
                                s3->v[1].u = sprStarIconRects[0].u1;
                                s3->v[1].v = sprStarIconRects[0].v1;
                                s3->z = 0;
                                s3++;
                            }
                            if (W->u76F8.recs[k].n == W->u76F8.recs[k].max) {
                                s3->type = 1;
                                s3->v[0].r = 0x80;
                                s3->v[0].g = 0x80;
                                s3->v[0].b = 0x80;
                                s3->v[0].a = 0;
                                s3->v[0].x = MN.ofs[k] + 0x104;
                                s3->v[0].y = i * 24 + 8;
                                s3->v[1].x = MN.ofs[k] + ((u16)sprStarIconRects[1].w + 0x104);
                                s3->v[1].y = i * 24 + ((u16)sprStarIconRects[1].h + 8);
                                s3->v[0].u = sprStarIconRects[1].u0;
                                s3->v[0].v = sprStarIconRects[1].v0;
                                s3->v[1].u = sprStarIconRects[1].u1;
                                s3->v[1].v = sprStarIconRects[1].v1;
                                s3->z = 0;
                                s3++;
                            }
                        } else {
                            s2 = FldMenuFractionBuild(s2, W->u76F8.recs[k].unkC, W->u76F8.recs[k].unkE, MN.ofs[k] + 0x100, i * 24 + 0xE);
                        }
                    } else {
                        FldMenuWinTextAddIndexed(MN.buf + 8, MN.buf, FONT->tbl[10], MN.ofs[k] + 0xE8, i * 24 + 0xC, 0x14, 0x14,
                                      W->books2[k].type, 3);
                        if (MN.timer > 0x80) {
                            for (jt = 0; jt < W->books2[k].n; jt++) {
                                s3->type = 1;
                                s3->v[0].r = 0x80;
                                s3->v[0].g = 0x80;
                                s3->v[0].b = 0x80;
                                s3->v[0].a = 0;
                                s3->v[0].x = jt * 16 + 0x100 + MN.ofs[k];
                                s3->v[0].y = i * 24 + 0xC;
                                s3->v[1].x = MN.bar[jt] + jt * 16 + 0x100 + MN.ofs[k];
                                s3->v[1].y = i * 24 + (MN.bar[jt] + 0xC);
                                s3->v[0].u = sprStarIconRects[0].u0;
                                s3->v[0].v = sprStarIconRects[0].v0;
                                s3->v[1].u = sprStarIconRects[0].u1;
                                s3->v[1].v = sprStarIconRects[0].v1;
                                s3->z = 0;
                                s3++;
                            }
                            if (W->books2[k].n == W->books2[k].max) {
                                s3->type = 1;
                                s3->v[0].r = 0x80;
                                s3->v[0].g = 0x80;
                                s3->v[0].b = 0x80;
                                s3->v[0].a = 0;
                                s3->v[0].x = MN.ofs[k] + 0x104;
                                s3->v[0].y = i * 24 + 8;
                                s3->v[1].x = MN.ofs[k] + ((u16)sprStarIconRects[1].w + 0x104);
                                s3->v[1].y = i * 24 + ((u16)sprStarIconRects[1].h + 8);
                                s3->v[0].u = sprStarIconRects[1].u0;
                                s3->v[0].v = sprStarIconRects[1].v0;
                                s3->v[1].u = sprStarIconRects[1].u1;
                                s3->v[1].v = sprStarIconRects[1].v1;
                                s3->z = 0;
                                s3++;
                            }
                        }
                    }
                    s3->type = 1;
                    s3->v[0].r = 0x80;
                    s3->v[0].g = 0x80;
                    s3->v[0].b = 0x80;
                    s3->v[0].a = 0;
                    s3->v[0].x = MN.ofs[k] + 4;
                    s3->v[0].y = i * 24 + 8;
                    s3->v[1].x = MN.ofs[k] + ((u16)menuSkillTypeIcons[icon].w + 4);
                    s3->v[1].y = i * 24 + ((u16)menuSkillTypeIcons[icon].h + 8);
                    s3->v[0].u = menuSkillTypeIcons[icon].u0;
                    s3->v[0].v = menuSkillTypeIcons[icon].v0;
                    s3->v[1].u = menuSkillTypeIcons[icon].u1;
                    s3->v[1].v = menuSkillTypeIcons[icon].v1;
                    s3->z = 0;
                    s3++;
                } else {
                    s5->type = 1;
                    s5->v[0].r = 0x80;
                    s5->v[0].g = 0x80;
                    s5->v[0].b = 0x80;
                    s5->v[0].a = 0;
                    s5->v[0].x = MN.ofs[k] + 4;
                    s5->v[0].y = i * 24 + 8;
                    s5->v[1].x = MN.ofs[k] + (menuItemIcons[icon].w + 4);
                    s5->v[1].y = i * 24 + (menuItemIcons[icon].h + 8);
                    s5->v[0].u = menuItemIcons[icon].u0;
                    s5->v[0].v = menuItemIcons[icon].v0;
                    s5->v[1].u = menuItemIcons[icon].u1;
                    s5->v[1].v = menuItemIcons[icon].v1;
                    s5->z = 0;
                    s5++;
                }
            }
        }
        p->type = 0;
        s5->type = 0;
        s2->type = 0;
        s3->type = 0;
        s4->type = 0;
        if (MN.state == 3 && FldMenuWinGetState(0xD) == 4) {
            if (MN.mode == 3) {
                switch (MN.tab) {
                case 0:
                    if (W->unk6ED4 != 0) {
                        MN.help = W->magic[MN.cursor2 + MN.scroll2].help;
                    }
                    MN.helpType = 3;
                    break;
                case 1:
                    if (W->books3Count != 0) {
                        MN.help = W->books3[MN.cursor2 + MN.scroll2].help;
                    }
                    MN.helpType = 3;
                    break;
                case 2:
                    if (W->books4Count != 0) {
                        MN.help = W->books4[MN.cursor2 + MN.scroll2].help;
                    }
                    MN.helpType = 2;
                    break;
                }
            } else {
                MN.toggle = 0;
                MN.help = 0;
                MN.last = -1;
            }
        } else if (MN.state != 0 || MN.helpType == 0xFFFFFFFF || FldMenuWinGetState(MN.buf + 8) != 4) {
            MN.last = -1;
            MN.help = 0;
            MN.toggle = 0;
        }
        if (MN.help != 0) {
            if (MN.state == 3) {
                if (MN.last != MN.cursor2 + MN.scroll2) {
                    MN.last = MN.cursor2 + MN.scroll2;
                    FldMenuCursorGetPos(&hx, &hy);
                    if (MN.toggle != 0) {
                        MenuHelpOpen(0, MN.helpType, MN.help, hx, hy + 16.0f);
                    } else {
                        MenuHelpOpen(1, MN.helpType, MN.help, 168.0f, 348.0f);
                    }
                }
            } else {
                if (MN.last != MN.cursor + MN.scroll) {
                    MN.last = MN.cursor + MN.scroll;
                    FldMenuCursorGetPos(&hx, &hy);
                    if (MN.toggle != 0) {
                        MenuHelpOpen(0, MN.helpType, MN.help, hx, hy + 16.0f);
                    } else {
                        MenuHelpOpen(1, MN.helpType, MN.help, 168.0f, 348.0f);
                    }
                }
            }
        } else {
            MN.last = -1;
            MenuHelpClose();
        }
        if (MN.mode == 3) {
            switch (MN.tab) {
            case 0:
                FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x20, 0x1E, 0x14, 0x14, n11++, 0x27, 8);
                break;
            case 1:
                FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x20, 0x1E, 0x14, 0x14, n11++, 0x26, 8);
                break;
            case 2:
                if (MN.state == 2) {
                    FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x20, 0x1E, 0x14, 0x14, n11++, 0x24, 8);
                } else {
                    if (W->shopChrB[W->u76F8.recs[MN.cursor + MN.scroll].owner].name != 0) {
                        FldMenuWinTextAdd(0xB, 2, W->shopChrB[W->u76F8.recs[MN.cursor + MN.scroll].owner].name, 0x10, 0x1E, 0x14,
                                      0x14, n11++, -1, 8);
                    }
                }
                break;
            }
            FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x18, 0x3E, 0x14, 0x14, n11++, 0x48, 3);
        } else {
            u8 tbl[16] = { 8, 9, 0x23, 0xB, 9, 0x11, 0x24, 0x13, 0x11, 0x3F, 0x1B, 0x1B, 0x19, 8, 0xA, 0xA };

            FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x20, 0x1E, 0x14, 0x14, n11++, tbl[MN.mode + MN.tab * 4] + MN.sel, 8);
            if (MN.mode == 1 && MN.tab != 3) {
                FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x30, 0x3E, 0x14, 0x14, n11++, 0x49, 3);
            } else if (MN.mode == 0 && MN.tab == 0) {
                FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x30, 0x3E, 0x14, 0x14, n11++, 7, 3);
            } else if (MN.mode == 2 && MN.tab == 0) {
                FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x30, 0x3E, 0x14, 0x14, n11++, 6, 3);
            } else if (MN.mode == 2 && MN.tab == 1) {
                FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x30, 0x3E, 0x14, 0x14, n11++, 7, 3);
            } else {
                FldMenuWinTextAdd(0xB, 2, FONT->tbl[15], 0x30, 0x3E, 0x14, 0x14, n11++, MN.mode + 6, 3);
            }
        }
    }
    FldMenuBarBuild(FldMenuWinGetQuadBuf(0xB), 8, 0x2E, 0xC0, 0xC, 0xA);
    p = FldMenuWinGetSprBuf(0xB, 1);
    FldMenuWinSetLayer(0xB, 1, &fldMenuTextures[0], p, 128.0f);
    FldMenuNumberBuild(p, m, 0, 0x94, 0x3E, 1.0f, 1.0f, 0);
    p = FldMenuWinGetSprBuf(0xB, 0);
    FldMenuWinSetLayer(0xB, 0, &fldMenuTextures[1], p, 128.0f);
    MN.blinkT++;
    if (MN.blinkT >= 2) {
        MN.blinkT = 0;
    }
    {
        u16 tbl2[4][4][8] = {
            { { 0x40, 0, 0, 0, 0, 0, 0, 0 },
              { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C },
              { 0x1F, 0x1D, 0x22, 0x25, 0x27, 0x29, 0x2C, 0x2E },
              { 0x31, 0x30, 0x3F, 0x3D, 0x38, 0, 0, 0 } },
            { { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C },
              { 0x1F, 0x1D, 0x22, 0x25, 0x27, 0x29, 0x2C, 0x2E },
              { 0x4C, 0x30, 0x3F, 0x3D, 0x38, 0, 0, 0 },
              { 0x40, 0, 0, 0, 0, 0, 0, 0 } },
            { { 0x40, 0, 0, 0, 0, 0, 0, 0 },
              { 0x4D, 0x41, 0, 0, 0, 0, 0, 0 },
              { 0, 0, 0, 0, 0, 0, 0, 0 },
              { 0, 0, 0, 0, 0, 0, 0, 0 } },
            { { 0x45, 0, 0, 0, 0, 0, 0, 0 },
              { 0x41, 0, 0, 0, 0, 0, 0, 0 },
              { 0x41, 0x41, 0x41, 0x41, 0x41, 0, 0, 0 },
              { 0x45, 0x41, 0x4D, 0, 0, 0, 0, 0 } },
        };

        for (i = 0; i < n; i++) {
            if (MN.state == 2 && MN.mode == 3) {
                v = MN.tab;
                icon = tbl2[MN.mode][3][i];
            } else if (MN.mode == 3 && MN.tab == 2) {
                if (W->shopChrB[i].name != 0) {
                    icon = W->shopChrB[i].icon;
                } else {
                    icon = 0x4E;
                }
                v = W->u76F8.recs[MN.cursor + MN.scroll].owner;
            } else {
                v = MN.sel;
                icon = tbl2[MN.mode][MN.tab][i];
            }
            p->type = 1;
            if (v == i) {
                p->v[0].r = 0x80;
                p->v[0].g = 0x80;
                p->v[0].b = 0x80;
                p->v[0].a = 0;
                if (MN.blinkT == 0) {
                    if (MN.blinkDir != 0) {
                        if (MN.pulse[i] >= -3) {
                            MN.pulse[i]--;
                        } else {
                            MN.blinkDir = 0;
                        }
                    } else {
                        if (MN.pulse[i] < 2) {
                            MN.pulse[i]++;
                        } else {
                            MN.blinkDir = 1;
                        }
                    }
                }
            } else {
                p->v[0].r = 0x80 - (MN.pulse[i] + 4) * 9;
                p->v[0].g = 0x80 - (MN.pulse[i] + 4) * 9;
                p->v[0].b = 0x80 - (MN.pulse[i] + 4) * 9;
                p->v[0].a = 0;
                if (MN.blinkT == 0 && MN.pulse[i] < 4) {
                    MN.pulse[i]++;
                }
            }
            p->v[0].x = i * 25 + 8 + MN.pulse[i] / 2;
            p->v[0].y = MN.pulse[i] + 4;
            p->v[1].x = i * 25 + (menuItemIcons[icon].w - MN.pulse[i]) + 8 + MN.pulse[i] / 2;
            p->v[1].y = MN.pulse[i] + 4 + (menuItemIcons[icon].h - MN.pulse[i]);
            p->v[0].u = menuItemIcons[icon].u0;
            p->v[0].v = menuItemIcons[icon].v0;
            p->v[1].u = menuItemIcons[icon].u1;
            p->v[1].v = menuItemIcons[icon].v1;
            p->z = 0;
            p++;
        }
    }
    p->type = 0;
    FldMenuWinTextClear(0xA);
    TxtMsgGetLength(FONT->tbl[15], MN.mode);
    w = TxtMsgGetWidth(FONT->tbl[15], MN.mode, 0x14);
    if (MN.mode == 3) {
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[15], (s32)(80.0f - (f32)w * 0.8f * 0.5f), -3, 0x10, 0x14, n11++, MN.mode, 1);
    } else {
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[15], 0x50 - w / 2, -3, 0x14, 0x14, n11++, MN.mode, 1);
    }
    if (MN.mode == 3 && MN.tab == 2) {
        w = TxtMsgGetWidth(FONT->tbl[15], 0x47, 0x14);
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[15], 0x1AA - w / 2, 8, 0x14, 0x14, n11++, 0x47, 2);
    } else if (MN.mode == 0) {
        u8 t[4] = { 3, 0, 1, 2 };

        w = TxtMsgGetWidth(FONT->tbl[15], MN.mode * 4 + 0x28 + t[MN.tab], 0x14);
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[15], 0x1AA - w / 2, 8, 0x14, 0x14, n11++, MN.mode * 4 + 0x28 + t[MN.tab], 2);
    } else {
        w = TxtMsgGetWidth(FONT->tbl[15], MN.mode * 4 + 0x28 + MN.tab, 0x14);
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[15], 0x1AA - w / 2, 8, 0x14, 0x14, n11++, MN.mode * 4 + 0x28 + MN.tab, 2);
    }
    if (MN.mode != 3) {
        if (MN.mode == 2) {
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[15], 0x84, 0x10A, 0x14, 0x14, n11++, 0x44, 2);
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[0], 0x148, 0x112, 0x10, 0x10, n11++, 0x35, 2);
        } else {
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[13], 0x84, 0x10A, 0x14, 0x14, n11++, 5, 2);
        FldMenuWinTextAdd(0xA, 2, FONT->tbl[0], 0x148, 0x112, 0x10, 0x10, n11++, 0x34, 2);
        }
        FldMenuBarBuild(FldMenuWinGetQuadBuf(0xA), 0x80, 0x118, 0xDC, 0xC, 1);
        p = FldMenuWinGetSprBuf(0xA, 1);
        FldMenuWinSetLayer(0xA, 0xC, &fldMenuTextures[0], p, 128.0f);
        MN.tick ^= 1;
        if ((MN.tick & 1) && MN.gold != gold) {
            if (gold < MN.gold) {
                d = MN.gold - gold;
                if (d >= 1000) {
                    MN.gold -= 1000;
                } else if (d >= 100) {
                    MN.gold -= 100;
                } else if (d >= 10) {
                    MN.gold -= 10;
                } else {
                    MN.gold -= 1;
                }
            } else {
                d = gold - MN.gold;
                if (d >= 1000) {
                    MN.gold += 1000;
                } else if (d >= 100) {
                    MN.gold += 100;
                } else if (d >= 10) {
                    MN.gold += 10;
                } else {
                    MN.gold += 1;
                }
            }
        }
        FldMenuNumberBuild(p, MN.gold, 1, 0x140, 0x10A, 1.0f, 1.0f, 0);
    }
    p = FldMenuWinGetSprBuf(0xA, 0);
    FldMenuWinSetLayer(0xA, 9, &fldMenuTextures[7], p, 128.0f);
    s5 = FldMenuWinGetSprBuf(0xA, 2);
    FldMenuWinSetLayer(0xA, 0xA, &fldMenuTextures[6], s5, 128.0f);
    s2 = FldMenuWinGetSprBuf(0xA, 3);
    FldMenuWinSetLayer(0xA, 0xB, &fldMenuTextures[8], s2, 128.0f);
    s3 = FldMenuWinGetSprBuf(0xA, 4);
    FldMenuWinSetLayer(0xA, 0xD, &fldMenuTextures[4], s3, 128.0f);
    if (MN.mode == 3) {
        for (i = 0; i < nChr; i++) {
            if (i == MN.chr) {
                MN.flags[i] = 1;
            } else {
                MN.flags[i] = 0;
            }
        }
    }
    for (i = 0; i < nChr; i++) {
        if (MN.state == 4) {
            MN.flags[i] = 0;
            diff[i] = 0;
        }
        FldMenuWinSetLayer(0xA, i + 1, ChrPortraitGetTex(i), &MN.win[i], 128.0f);
        if (MN.flags[i] == 1) {
            if (MN.slide[i] < 0) {
                MN.slide[i] += 4;
                if (MN.slide[i] >= 0) {
                    MN.slide[i] = 0;
                }
            }
        } else if (MN.slide[i] >= -0x17) {
            MN.slide[i] -= 4;
            if (MN.slide[i] < -0x17) {
                MN.slide[i] = -0x18;
            }
        }
        if (MN.scale[i] >= 3.0f) {
            if (MN.cnt[i] != 0) {
                MN.cnt[i]--;
                MN.scale[i] -= 2.0f;
                MN.vel[i] *= 0.7f;
            } else {
                MN.scale[i] = 3.0f;
            }
        } else {
            MN.scale[i] += MN.vel[i];
        }
        if (MN.scale[i] != 3.0f) {
            MN.slide[i] = 0;
        } else {
            MN.tim[i]++;
            if (MN.tim[i] >= 60) {
                MN.tim[i] = 0;
            }
            if (diff[i] != 0 && MN.slide[i] == 0) {
                u32 t = MN.tim[i];

                if (t > 0x10) {
                    t = 0;
                }
                s5->type = 1;
                s5->v[0].r = 0x80;
                s5->v[0].g = 0x80;
                s5->v[0].b = 0x80;
                s5->v[0].a = 0;
                s5->v[0].x = MN.slide[i] + (t / 2 + 0x4D);
                s5->v[0].y = i * 40 + 0x2D;
                s5->v[1].x = MN.slide[i] + 0x5D + t / 2 - t;
                s5->v[1].y = i * 40 + 0x3D;
                s5->z = 0;
                if (diff[i] > 0) {
                    s5->v[0].u = 0xDC;
                    s5->v[0].v = 0xDA;
                    s5->v[1].u = 0xEC;
                    s5->v[1].v = 0xEA;
                } else {
                    s5->v[0].u = 0xEE;
                    s5->v[0].v = 0xDA;
                    s5->v[1].u = 0xFE;
                    s5->v[1].v = 0xEA;
                }
                s5++;
                if (MN.val[i] != diff[i]) {
                    if (diff[i] < MN.val[i]) {
                        MN.val[i]--;
                    } else {
                        MN.val[i]++;
                    }
                }
                if (MN.val[i] > 0) {
                    s2 = FldMenuNumber2Build(s2, MN.val[i] < 0 ? -MN.val[i] : MN.val[i], 0, MN.slide[i] + 0x5E, i * 40 + 0x20, 1.0f, 1.0f, eq6 * 2);
                } else {
                    s2 = FldMenuNumber2Build(s2, MN.val[i] < 0 ? -MN.val[i] : MN.val[i], 0, MN.slide[i] + 0x5E, i * 40 + 0x20, 1.0f, 1.0f, eq6 * 2 + 1);
                }
            }
        }
        f = FldMenuWobbleCurve(MN.scale[i]);
        if (ChrIsInParty(i + 1) == 1) {
            for (j = 0; j < 3; j++) {
                VuSinCosDeg(&sn3, &cs3, (f32)j * 110.0f + (f32)MN.ang);
                s3->type = 1;
                s3->v[0].r = 0x80;
                s3->v[0].g = 0x80;
                s3->v[0].b = 0x80;
                s3->v[0].a = 0;
                s3->v[0].x = MN.slide[i] + 0x10 + (s32)(cs3 * 12.0f);
                s3->v[0].y = i * 40 + 0x18 + (s32)(sn3 * 6.0f);
                s3->v[1].x = MN.slide[i] + ((s32)(cs3 * 12.0f) + 0x1A);
                s3->v[1].y = i * 40 + ((s32)(sn3 * 6.0f) + 0x22);
                s3->v[0].u = sprStarIconRects[0].u0;
                s3->v[0].v = sprStarIconRects[0].v0;
                s3->v[1].u = sprStarIconRects[0].u1;
                s3->v[1].v = sprStarIconRects[0].v1;
                s3->z = 0;
                s3++;
            }
            if (MN.ang >= 0x5B) {
                FldMenuWinTextAdd(0xA, 2, FONT->tbl[8], MN.slide[i], i * 40 + 0x18, 0x10, 0x10, 0x1B, 0x19, 1);
            }
        }
        p = FldMenuShopCharPlateBuild(p, i, MN.slide[i], i * 40 + 0x30, MN.slide[i], 1.0f, f);
    }
    p->type = 0;
    s5->type = 0;
    s2->type = 0;
    {
        u8 cnt[4] = { 3, 3, 1, 2 };

        for (i = 0; i < cnt[MN.mode] + 1; i++) {
            if (MN.mode == 3) {
                if (i == MN.sel2) {
                    FldMenuWinTextAdd(0xE, MN.buf, FONT->tbl[15], 0x20, i * 24 + 0x10, 0x14, 0x14, n8++, i + 0x45, 8);
                } else {
                    FldMenuWinTextAdd(0xE, MN.buf, FONT->tbl[15], 0x20, i * 24 + 0x10, 0x14, 0x14, n8++, i + 0x45, 0);
                }
            } else if (i == MN.sel2) {
                FldMenuWinTextAdd(0xE, MN.buf, FONT->tbl[15], 0x20, i * 24 + 0x10, 0x14, 0x14, n8++, MN.mode * 4 + 0x28 + i, 8);
            } else {
                FldMenuWinTextAdd(0xE, MN.buf, FONT->tbl[15], 0x20, i * 24 + 0x10, 0x14, 0x14, n8++, MN.mode * 4 + 0x28 + i, 0);
            }
        }
    }
    }
    return 0xF;
}
