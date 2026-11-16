/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "field/n24.h"
#include "field/n23.h"

typedef struct {
    s32 flag;
} Box;

typedef struct {
    u8 pad0[0x94];
    s32 x94;
} Tex;

extern u8 fldFade1[];

void ItemSkillItemListBuild(void);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);
void MemMove128(s32, s32, s32);
void GphRippleApply8(s32);
FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
void VuBezier3(f32 *, f32 (*)[4], f32);

s32 FldMenuSkillmanUpdate(void) {
    f32 out[4];
    f32 pts[4][4];
    f32 fa;
    f32 fb;
    s32 n;
    s32 r;
    s32 id;
    s32 idx;
    s32 i;
    s32 c;
    s32 y;
    s32 w;
    s8 ic;
    FldMenuQuad *s;
    Box *h;
    Tex *tx;
    u16 b;
    s32 k;
    s32 xx;

    ItemSkillItemListBuild();
    n = itemWork->saleCount;
    r = ItemSkillHasRoom();
    if (r < 0) {
        n = 0;
    }
    FldMenuWinTextClear(8);
    FldMenuWinTextClear(0xA);
    FldMenuWinTextClear(0xB);
    FldMenuWinTextClear(9);
    id = 0;
    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[13], 0, -8, 0x20, 0x20, id++, 0, 1);
    if (n == 0) {
        if (r >= 0) {
            FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[13], 0x154, 0xEC, 0x14, 0x14, id++, 0xB, 8);
        } else {
            FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[13], 0x100, 0xE0, 0x14, 0x14, id++, 0xD, 8);
            FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[13], 0x140, 0xF4, 0x14, 0x14, id++, 0xE, 8);
        }
    }
    FldMenuWinTextAdd(0xA, 0, sysGrxsysPac->msg->tbl[13], 8, 8, 0x14, 0x14, id++, 2, 3);
    FldMenuWinTextAdd(0xA, 0, sysGrxsysPac->msg->tbl[13], 0xB6, 8, 0x14, 0x14, id++, 0xF, 3);
    FldMenuWinTextAdd(0xA, 0, sysGrxsysPac->msg->tbl[13], 8, 0x20, 0x14, 0x14, id++, 3, 3);
    if (fldMenuSkillman.sub != 0) {
        FldMenuWinTextAdd(0xA, 0, sysGrxsysPac->msg->tbl[13], 0x84, 0x6C, 0x14, 0x14, id++, 7, 0);
        FldMenuWinTextAdd(0xA, 0, sysGrxsysPac->msg->tbl[13], 0x84, 0x82, 0x14, 0x14, id++, 8, 8);
    } else {
        FldMenuWinTextAdd(0xA, 0, sysGrxsysPac->msg->tbl[13], 0x84, 0x6C, 0x14, 0x14, id++, 7, 8);
        FldMenuWinTextAdd(0xA, 0, sysGrxsysPac->msg->tbl[13], 0x84, 0x82, 0x14, 0x14, id++, 8, 0);
    }

    switch (fldMenuSkillman.state) {
    case 0:
        if (n != 0) {
            b = padPort0[0].repeat;
            if (b & 0x1000) {
                if (fldMenuSkillman.page != 0) {
                    SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                    fldMenuSkillman.page = 0;
                }
            } else if (b & 0x4000) {
                if (fldMenuSkillman.page == 0) {
                    SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                    if (n >= 0x10) {
                        fldMenuSkillman.page = 1;
                        if (fldMenuSkillman.cursor >= n - 0xF) {
                            fldMenuSkillman.cursor = n - 0x10;
                        }
                    }
                }
            } else if (b & 0x8000) {
                if (fldMenuSkillman.cursor != 0) {
                    fldMenuSkillman.cursor--;
                } else if (n < 0x10) {
                    fldMenuSkillman.cursor = n - 1;
                } else if (fldMenuSkillman.page != 0) {
                    fldMenuSkillman.page = 0;
                    fldMenuSkillman.cursor = 0xE;
                } else {
                    fldMenuSkillman.page = 1;
                    fldMenuSkillman.cursor = n - 0x10;
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            } else if (b & 0x2000) {
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                if (fldMenuSkillman.page == 0) {
                    if (n >= 0x10) {
                        if (fldMenuSkillman.cursor < 0xE) {
                            fldMenuSkillman.cursor++;
                        } else {
                            fldMenuSkillman.cursor = 0;
                            fldMenuSkillman.page = 1;
                        }
                    } else {
                        if (fldMenuSkillman.cursor < n - 1) {
                            fldMenuSkillman.cursor++;
                        } else {
                            fldMenuSkillman.page = 0;
                            fldMenuSkillman.cursor = 0;
                        }
                    }
                } else {
                    if (fldMenuSkillman.cursor < n - 0x10) {
                        fldMenuSkillman.cursor++;
                    } else {
                        fldMenuSkillman.page = 0;
                        fldMenuSkillman.cursor = 0;
                    }
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            } else if (b & 0x20) {
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                fldMenuSkillman.sub = 1;
                fldMenuSkillman.state = 1;
                FldMenuWinOpen(0xA, 0x140, 0x100, 0x168, 0xA8, 0, 1, 0x400000);
            } else if (padPort0[0].trigger & 0x10) {
                if (fldMenuSkillman.toggle != 0) {
                    fldMenuSkillman.toggle = 0;
                } else {
                    fldMenuSkillman.toggle = 1;
                }
                fldMenuSkillman.last = -1;
            } else {
                goto check; /* shares the cancel test with the n == 0 path */
            }
            break;
        }
    check:
        if (padPort0[0].repeat & 0x40) {
            FldMenuClose();
        }
        break;
    case 1:
        b = padPort0[0].repeat;
        if (b & 0x1000) {
            if (fldMenuSkillman.sub != 0) {
                fldMenuSkillman.sub = 0;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        } else if (b & 0x4000) {
            if (fldMenuSkillman.sub == 0) {
                fldMenuSkillman.sub = 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        } else if (b & 0x20) {
            if (fldMenuSkillman.sub == 0) {
                if (itemWork->gold >= itemWork->sale[fldMenuSkillman.cursor + fldMenuSkillman.page * 15].price) {
                    fldMenuSkillman.state = 2;
                    FldMenuWinClose(0xA, 0, 0);
                    FldMenuWinClose(8, 0, 1);
                    fldMenuSkillman.timer = 0;
                    fldMenuSkillman.unk34 = 0.0f;
                    PtySetGold(itemWork->gold - itemWork->sale[fldMenuSkillman.cursor + fldMenuSkillman.page * 15].price);
                    tx = FldOverlayImgGetTex(fldMenuSkillman.item + 2);
                    MemMove128(FldMenuGetImageBuf(), tx->x94, 0x400);
                    GphRippleCreate(0x80, 0x80, FldMenuGetImageBuf(), tx->x94, fldMenuSkillman.ripple);
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            } else {
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                fldMenuSkillman.state = 0;
                FldMenuWinClose(0xA, 0, 3);
            }
        } else if (b & 0x40) {
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            fldMenuSkillman.state = 0;
            FldMenuWinClose(0xA, 0, 3);
        }
        break;
    case 2:
        FldMenuCursorReset();
        if (++fldMenuSkillman.timer == 0x1E) {
            SndSePlay(0x800, 0, 0, 0x1E, 0);
        }
        if (fldMenuSkillman.timer >= 0x1E) {
            fldMenuSkillman.alpha24 += 2;
            if (fldMenuSkillman.alpha24 >= 0x80) {
                fldMenuSkillman.alpha24 = 0x80;
            }
        }
        if (fldMenuSkillman.timer == 0x4E) {
            GphFadeStart(fldFade1, 0, 255.0f, 255.0f, 255.0f, 64.0f, 8.0f);
        }
        if (fldMenuSkillman.timer == 0x58) {
            GphFadeStart(fldFade1, 0, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f);
        }
        if (fldMenuSkillman.timer >= 0x4E) {
            fldMenuSkillman.alpha26 += 1;
            if (fldMenuSkillman.alpha26 >= 0x80) {
                fldMenuSkillman.alpha26 = 0x80;
            }
        }
        if (fldMenuSkillman.timer < 8) {
            c = VuRandInt(0x80);
            GphRippleAddRect(fldMenuSkillman.ripple, c, VuRandInt(0x80), 8, 8, 0x5A);
        }
        if (fldMenuSkillman.timer == 0x12C) {
            FldMenuWinOpen(0xB, 0x100, 0xE0, 0x1E0, 0x40, 0, 0, 0x400000);
            fldMenuSkillman.state = 3;
            MenuHelpOpen(0, 2, itemWork->sale[fldMenuSkillman.cursor + fldMenuSkillman.page * 15].msg, 128.0f, 288.0f);
            fldMenuSkillman.timer = 0;
            fldMenuSkillman.t = 0.0f;
        }
        break;
    case 3:
        if (fldMenuSkillman.timer < 0x78) {
            fldMenuSkillman.timer++;
        }
        fldMenuSkillman.alpha24 -= 2;
        if (fldMenuSkillman.alpha24 < 0) {
            fldMenuSkillman.alpha24 = 0;
        }
        if ((padPort0[0].trigger & 0x20) && fldMenuSkillman.alpha24 == 0 && fldMenuSkillman.t >= 1.0f && FldMenuWinGetState(0xB) == 4) {
            FldMenuWinClose(0xB, 0, 1);
            fldMenuSkillman.toggle = 0;
            fldMenuSkillman.help = 0;
            fldMenuSkillman.last = -1;
        }
        if (FldMenuWinGetState(0xB) == 0) {
            ItemSkillAdd(itemWork->sale[fldMenuSkillman.cursor + fldMenuSkillman.page * 15].id);
            ItemBagRemove((s8)itemWork->sale[fldMenuSkillman.cursor + fldMenuSkillman.page * 15].unk4, 1);
            fldMenuSkillman.t = 0.0f;
            SndSePlay(0x7FF, 0, 0, 0x1E, 0);
            fldMenuSkillman.state = 0;
            fldMenuSkillman.page = 0;
            fldMenuSkillman.cursor = 0;
            FldMenuWinOpen(8, 0x112, 0x5A, 0x1E0, 0x78, 0, 0, 0x400000);
            ItemSkillItemListBuild();
        }
        break;
    }

    if (fldMenuSkillman.state >= 2) {
        GphRippleUpdate(fldMenuSkillman.ripple, 0xB);
        GphRippleApply8(fldMenuSkillman.ripple);
    }
    if (n != 0) {
        switch (fldMenuSkillman.state) {
        case 0:
            FldMenuCursorSetAt(8, (f32)(fldMenuSkillman.cursor * 0x1E) + 8.0f, (f32)(fldMenuSkillman.page * 0x22) + 40.0f);
            break;
        case 1:
            FldMenuCursorSetAt(0xA, (f32)(fldMenuSkillman.sub * 0xF) + 128.0f, (f32)(fldMenuSkillman.sub * 0x14) + 122.0f);
            break;
        }
    }
    idx = fldMenuSkillman.cursor + fldMenuSkillman.page * 15;
    if (n != 0) {
        fldMenuSkillman.item = itemWork->sale[idx].id - 1;
        if (fldMenuSkillman.state == 0) {
            fldMenuSkillman.help = itemWork->sale[idx].help;
        } else {
            fldMenuSkillman.help = 0;
        }
    } else {
        fldMenuSkillman.help = 0;
        fldMenuSkillman.item = 0;
    }
    if (fldMenuSkillman.state != 3) {
        if (fldMenuSkillman.help != 0) {
            if (fldMenuSkillman.last != idx) {
                fldMenuSkillman.last = idx;
                FldMenuCursorGetPos(&fa, &fb);
                if (fldMenuSkillman.toggle != 0) {
                    if (fldMenuSkillman.state == 0) {
                        MenuHelpOpen(0, 3, fldMenuSkillman.help, 64.0f, 128.0f);
                    } else {
                        MenuHelpOpen(0, 2, fldMenuSkillman.help, 48.0f, 144.0f);
                    }
                } else {
                    if (fldMenuSkillman.state == 0) {
                        MenuHelpOpen(1, 3, fldMenuSkillman.help, 32.0f, 348.0f);
                    } else {
                        MenuHelpOpen(1, 2, fldMenuSkillman.help, 32.0f, 348.0f);
                    }
                }
            }
        } else {
            fldMenuSkillman.last = -1;
            MenuHelpClose();
        }
    }
    if (n != 0) {
        FldMenuWinTextAdd(0xA, 0, itemWork->sale[idx].name, 0x60, 0x44, 0x14, 0x14, id++, -1, 8);
    }
    FldMenuBarBuild(FldMenuWinGetQuadBuf(0xA), 0x36, 0x54, 0x120, 0xC, 1);
    s = FldMenuWinGetSprBuf(0xA, 0);
    FldMenuWinSetLayer(0xA, 0, &fldMenuTextures[1], s, 128.0f);
    if (n != 0) {
        ic = itemWork->sale[idx].icon;
        s->type = 1;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = 0;
        s->z = 0;
        s->v[0].x = 0x3C;
        s->v[0].y = 0x40;
        s->v[1].x = menuItemIcons[ic].w + 0x3C;
        s->v[1].y = menuItemIcons[ic].h + 0x40;
        s->v[0].u = menuItemIcons[ic].u0;
        s->v[0].v = menuItemIcons[ic].v0;
        s->v[1].u = menuItemIcons[ic].u1;
        s->v[1].v = menuItemIcons[ic].v1;
        s++;
    }
    s->type = 0;
    if (n != 0) {
        FldMenuWinTextAdd(8, 0, itemWork->sale[idx].name, 0xC0, 4, 0x14, 0x14, id++, -1, 8);
        FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[13], 0x10, 0x62, 0x14, 0x14, id++, 4, 2);
    }
    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[13], 0x122, 0x62, 0x14, 0x14, id++, 5, 3);
    s = (FldMenuQuad *)FldMenuWinGetQuadBuf(8);
    if (n != 0) {
        s = (FldMenuQuad *)FldMenuBarBuild((Box *)s, 0xA8, 0x14, 0x140, 0xC, 1);
        s = (FldMenuQuad *)FldMenuBarBuild((Box *)s, 8, 0x72, 0xD2, 0xC, 0xA);
    }
    FldMenuBarBuild((Box *)s, 0x104, 0x72, 0x100, 0xC, 0xA);
    s = FldMenuWinGetSprBuf(8, 0);
    FldMenuWinSetLayer(8, 0, &fldMenuTextures[0], s, 128.0f);
    if (n != 0) {
        s = (FldMenuQuad *)FldMenuNumberBuild(s, itemWork->sale[idx].price, 1, 0xC8, 0x62, 1.0f, 1.0f, 0);
        FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[0], 0xC8, 0x68, 0x10, 0x10, id++, 0x34, 2);
    }
    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[0], 0x1F4, 0x68, 0x10, 0x10, id++, 0x34, 2);
    *((s32 *(*)(void *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(s, itemWork->gold, 1, 0x1F4, 0x62, 1.0f, 1.0f, 0) = 0;
    s = FldMenuWinGetSprBuf(8, 1);
    FldMenuWinSetLayer(8, 1, &fldMenuTextures[1], s, 128.0f);
    for (i = 0; i < n; i++) {
        ic = itemWork->sale[i].icon;
        s->type = 1;
        if (i == idx) {
            s->v[0].a = 0;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
        } else {
            s->v[0].a = 0;
            s->v[0].r = 0x40;
            s->v[0].g = 0x40;
            s->v[0].b = 0x40;
        }
        if (i >= 0xF) {
            s->v[0].y = 0x40;
            s->z = 0;
            s->v[0].x = (i - 0xF) * 0x1E + 8;
            s->v[1].x = (i - 0xF) * 0x1E + (menuItemIcons[ic].w + 8);
            s->v[1].y = menuItemIcons[ic].h + 0x40;
        } else {
            s->v[0].y = 0x22;
            s->z = 0;
            s->v[0].x = i * 0x1E + 8;
            s->v[1].x = i * 0x1E + (menuItemIcons[ic].w + 8);
            s->v[1].y = menuItemIcons[ic].h + 0x22;
        }
        s->v[0].u = menuItemIcons[ic].u0;
        s->v[0].v = menuItemIcons[ic].v0;
        s->v[1].u = menuItemIcons[ic].u1;
        s->v[1].v = menuItemIcons[ic].v1;
        s++;
    }
    s->type = 0;

    xx = 0xE0;
    s = FldMenuWinGetSprBuf(9, 2);
    k = 0x80;
    FldMenuWinSetLayer(9, 2, FldOverlayImgGetTex(0), s, 128.0f);
    s->type = 1;
    s->v[0].r = k;
    s->v[0].g = k;
    s->v[0].b = k;
    s->v[0].a = 0;
    s->v[0].x = xx;
    s->z = 0;
    s->v[0].y = k;
    s->v[1].x = 0x220;
    s->v[1].y = 0x180;
    s->v[0].u = 0;
    s->v[0].v = 0;
    s->v[1].u = 0x100;
    s->v[1].v = 0xD2;
    s[1].type = 0;
    if (fldMenuSkillman.state != 3) {
        if (n != 0) {
            fldMenuSkillman.t += 0.02f;
        }
        if (fldMenuSkillman.t >= 1.0f) {
            fldMenuSkillman.t = 1.0f;
        }
        pts[0][0] = -128.0f;
        pts[3][1] = 128.0f;
        pts[1][0] = 0.0f;
        pts[2][0] = 0.0f;
        pts[3][0] = 0.0f;
        pts[0][1] = 0.0f;
        pts[1][1] = 0.0f;
        pts[2][1] = 0.0f;
        VuBezier3(out, pts, fldMenuSkillman.t);
    } else {
        if (fldMenuSkillman.timer >= 0x3C) {
            fldMenuSkillman.t += 0.02f;
            if (fldMenuSkillman.t >= 1.0f) {
                fldMenuSkillman.t = 1.0f;
            }
        }
        pts[0][0] = 0.0f;
        pts[1][0] = 0.0f;
        pts[2][0] = 0.0f;
        pts[3][0] = 128.0f;
        pts[0][1] = 128.0f;
        pts[1][1] = 0.0f;
        pts[2][1] = 0.0f;
        pts[3][1] = 0.0f;
        VuBezier3(out, pts, fldMenuSkillman.t);
    }

    s = FldMenuWinGetSprBuf(9, 4);
    FldMenuWinSetLayerVisible(9, 4, 0);
    FldMenuWinSetLayer(9, 4, FldOverlayImgGetTex(0x44), s, 128.0f);
    s->type = 1;
    s->v[0].r = 0x80;
    s->v[0].g = 0x80;
    s->v[0].b = 0x80;
    s->v[0].a = (s32)out[1];
    s->z = 0;
    s->v[0].x = xx + 0x20;
    s->v[0].y = (s32)out[0] + 0xA0;
    s->v[1].x = 0x200;
    s->v[1].y = (s32)out[0] + 0x168;
    s->v[0].u = 0;
    s->v[0].v = 0;
    s->v[1].u = 0x100;
    s->v[1].v = 0xC8;
    s[1].type = 0;
    if (fldMenuSkillman.state >= 2) {
        s = FldMenuWinGetSprBuf(9, 3);
        FldMenuWinSetLayerVisible(9, 3, 0);
        FldMenuWinSetLayer(9, 3, FldOverlayImgGetTex(1), s, 128.0f);
        s->type = 2;
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = fldMenuSkillman.alpha24;
        s->z = 0;
        s->v[0].x = 0xF0;
        s->v[0].y = 0x90;
        s->v[1].x = 0x21C;
        s->v[1].y = 0x176;
        s->v[0].u = 0;
        s->v[0].v = 0;
        s->v[1].u = 0x80;
        s->v[1].v = 0x80;
        s[1].type = 0;
        s = FldMenuWinGetSprBuf(9, 5);
        FldMenuWinSetLayerVisible(9, 5, 0);
        FldMenuWinSetLayer(9, 5, FldOverlayImgGetTex(fldMenuSkillman.item + 2), s, 128.0f);
        if (fldMenuSkillman.state >= 3) {
            fldMenuSkillman.alpha26 = (s32)out[1];
        }
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->type = 1;
        s->v[0].b = 0x80;
        s->v[0].a = fldMenuSkillman.alpha26;
        s->v[0].x = 0x140;
        s[1].type = 0;
        s->z = 0;
        s->v[0].u = 0;
        s->v[0].v = 0;
        s->v[1].x = xx + 0xE0;
        s->v[1].u = 0x80;
        s->v[0].y = (s32)out[0] + 0xC0;
        s->v[1].v = 0x80;
        s->v[1].y = (s32)out[0] + 0x140;
    }
    if (n != 0) {
        FldMenuWinTextAdd(0xB, 0, itemWork->sale[idx].desc, 0x34, 8, 0x14, 0x14, id++, -1, 2);
        w = TxtStrGetWidth(itemWork->sale[idx].desc, 0x14);
        FldMenuWinTextAdd(0xB, 0, sysGrxsysPac->msg->tbl[13], w + 0x34, 8, 0x14, 0x14, id++, 0xA, 8);
        FldMenuBarBuild(FldMenuWinGetQuadBuf(0xB), 0x10, 0x18, w + 0xD4, 0xC, 1);
        s = FldMenuWinGetSprBuf(0xB, 1);
        FldMenuWinSetLayer(0xB, 1, &fldMenuTextures[4], s, 128.0f);
        ic = itemWork->sale[idx].icon2;
        s->v[1].x = menuSkillTypeIcons[ic].w + 0x10;
        s->v[1].y = menuSkillTypeIcons[ic].h + 8;
        s->v[0].r = 0x80;
        s->type = 1;
        s->v[0].g = 0x80;
        s->v[0].u = menuSkillTypeIcons[ic].u0;
        s->v[0].b = 0x80;
        s->v[0].v = menuSkillTypeIcons[ic].v0;
        s->v[0].x = 0x10;
        s->v[1].u = menuSkillTypeIcons[ic].u1;
        s->v[1].v = menuSkillTypeIcons[ic].v1;
        s->v[0].a = 0;
        s->v[0].y = 8;
        s->z = 0;
        s[1].type = 0;
    }
    return 0xE;
}
