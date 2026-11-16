/* Port-only C for FldMenuMagicUpdate (0x0062F360), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "field/menu.h"
#include "field/n23.h"

/*
 * Based on the decompilation rounds' near miss (1879 of 1879 instructions,
 * 9 words different, all scheduling in the L button branch).
 */

/* itemWork->eggSpells[chr] as the magic screen reads it (0x98 bytes). */
typedef struct MagicSpell {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ s8 icon;
    /* 0x02 */ s16 num;
    /* 0x04 */ s8 kind;
    /* 0x05 */ s8 col;
    /* 0x06 */ s8 slot[6];
    /* 0x0C */ s32 name;
    /* 0x10 */ s32 help;
} MagicSpell; /* size 0x14 */

typedef struct MagicChr {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s8 id;
    /* 0x03 */ s8 icon;
    /* 0x04 */ s8 cnt;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 name;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 orbs;
    /* 0x16 */ s16 lv;
    /* 0x18 */ MagicSpell e[6];
    /* 0x90 */ u8 pad90[8];
} MagicChr; /* size 0x98 */

/* itemWork viewed with eggSpells as MagicChr records. */
typedef struct MagicWork {
    /* 0x0000 */ u8 pad0[0x4A8C];
    /* 0x4A8C */ MagicChr m[24];
} MagicWork;

#define MCHR(c) (((MagicWork *)itemWork)->m[c])

typedef struct FldMenuMagic {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 sel;
    /* 0x02 */ u8 dir;
    /* 0x03 */ s8 state;
    /* 0x04 */ u8 pad4;
    /* 0x05 */ u8 chr;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 cur;
    /* 0x0C */ s8 offs[16];
    /* 0x1C */ u8 help;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ s32 last;
    /* 0x24 */ s32 helpId;
    /* 0x28 */ u8 page;
    /* 0x29 */ u8 pad29[3];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 blink;
    /* 0x35 */ u8 bdir;
    /* 0x36 */ s8 bob[24];
    /* 0x4E */ u8 idx;
    /* 0x4F */ u8 bars[8];
    /* 0x57 */ u8 timer;
    /* 0x58 */ s16 angle;
    /* 0x5A */ u8 pad5A[2];
} FldMenuMagic; /* size 0x5C */

extern FldMenuMagic fldMenuMagic;

void MenuLrArrowsFlash(s8);
void MenuHelpClose(void);
void VuSinCosDeg(f32 *, f32 *, f32);
s32 ItemEggSpellCanCast(s16, s16, s16);
s32 ItemEggSpellCastField(s32, s32, s16, s16);
void FldMenuCursorSetAt(u8, f32, f32);
FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
void FldMenuCursorGetPos(f32 *, f32 *);
void FldMenuLrArrowsUnlock(void);
void FldMenuWinTextClear(u8);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);
void FldMenuWinOpen(u8, s16, s16, u16, u16, u16, u8, s32);
s32 FldMenuWinGetState(u8);
void FldMenuWinLrArrowsSet(u8, u8, f32);
void FldMenuWinClose(u8, u8, s32);
void FldMenuWinSetActive(u8);
FldMenuQuad *FldMenuHpFractionBuild(FldMenuQuad *, s32, s32, s32, s32);
void FldMenuPartyPanelFlash(u8);
void FldMenuPartyPanelOpen(s32, s32, s32);
s32 FldMenuPartyPanelUpdate(u8);
void FldMenuListCursorDown(u32 *, u32 *, u32, u32, u32);
void FldMenuListCursorUp(u32 *, u32 *, u32, u32, u32);
s32 FldMenuPartySelect(u8, s32 *, s32 *);
FldMenuQuad *FldMenuBarBuild(FldMenuQuad *, s32, s32, s32, s32, s32);

s32 FldMenuMagicUpdate(void) {
    f32 cs;
    f32 sn;
    f32 y;
    f32 x;
    s32 top;
    s32 n;
    s32 page2;
    s32 chr;
    s32 cnt;
    s32 k;
    FldMenuQuad *p0;
    FldMenuQuad *p2;
    s32 party;
    s32 i;
    s32 j;
    s32 ic;
    s32 col;
    s32 h;
    u8 pg;
    s32 xx;
    s32 t;
    FldMenuQuad *p1;
    FldMenuQuad *p3;
    FldMenuQuad *q;

    chr = fldMenuMagic.chr;
    top = 0;
    cnt = MCHR(chr).cnt;
    party = (u16)itemWork->unk6C6C;
    n = 0;
    page2 = fldMenuMagic.page ^ 1;
    k = 0;
    fldMenuMagic.angle += 5;
    if (fldMenuMagic.angle >= 360) {
        fldMenuMagic.angle -= 360;
    }
    for (i = 0; i < 8; i++) {
        if (fldMenuMagic.idx == i) {
            if (fldMenuMagic.bars[i] < (s16)fldMenuJunctionIcons[0].h / 2) {
                fldMenuMagic.bars[i]++;
            } else {
                fldMenuMagic.bars[i]--;
            }
            if (fldMenuMagic.bars[i] == (s16)fldMenuJunctionIcons[0].h / 2) {
                fldMenuMagic.idx++;
                if (fldMenuMagic.idx >= 7) {
                    fldMenuMagic.idx = 0;
                }
            }
        } else if (fldMenuMagic.bars[i] < (s16)fldMenuJunctionIcons[0].h) {
            fldMenuMagic.bars[i] += 2;
            if (fldMenuMagic.bars[i] >= (s16)fldMenuJunctionIcons[0].h) {
                fldMenuMagic.bars[i] = fldMenuJunctionIcons[0].h;
            }
        }
    }

    switch (fldMenuMagic.state) {
    case 0:
        if (padPort0[0].trigger & 0x40) {
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            return 1;
        }
        if (party != 0) {
            if (padPort0[0].trigger & 4) {
                t = fldMenuMagic.sel != 0 ? fldMenuMagic.sel - 1 : party - 1;
                fldMenuMagic.sel = t;
                SndSePlay(0x7D5, 0, 0, 0x1E, 0);
                do {} while (0);
                MenuLrArrowsFlash(1);
                fldMenuMagic.last = -1;
                fldMenuMagic.cur = 0;
                fldMenuMagic.dir = 0;
                FldMenuWinClose(fldMenuMagic.page + 8, 0, fldMenuMagic.dir);
                fldMenuMagic.page ^= 1;
                switch (fldMenuMagic.dir) {
                case 0:
                    FldMenuWinOpen(fldMenuMagic.page + 8, 0x140, 0xD4, 0x21C, 0xA4, 0, 1, 0x88000);
                    break;
                case 1:
                    FldMenuWinOpen(fldMenuMagic.page + 8, 0x140, 0xD4, 0x21C, 0xA4, 0, 0, 0x88000);
                    break;
                }
                fldMenuMagic.last = -1;
                fldMenuMagic.unk0 = fldMenuMagic.sel;
                fldMenuMagic.chr = fldMenuMagic.sel;
                fldMenuMagic.help = 0;
            } else if (padPort0[0].trigger & 8) {
                if (fldMenuMagic.sel < party - 1) {
                    fldMenuMagic.sel++;
                } else {
                    fldMenuMagic.sel = 0;
                }
                SndSePlay(0x7D5, 0, 0, 0x1E, 0);
                MenuLrArrowsFlash(0);
                fldMenuMagic.last = -1;
                fldMenuMagic.dir = 1;
                fldMenuMagic.cur = 0;
                FldMenuWinClose(fldMenuMagic.page + 8, 0, 1);
                fldMenuMagic.page ^= 1;
                switch (fldMenuMagic.dir) {
                case 0:
                    FldMenuWinOpen(fldMenuMagic.page + 8, 0x140, 0xD4, 0x21C, 0xA4, 0, 1, 0x88000);
                    break;
                case 1:
                    FldMenuWinOpen(fldMenuMagic.page + 8, 0x140, 0xD4, 0x21C, 0xA4, 0, 0, 0x88000);
                    break;
                }
                fldMenuMagic.last = -1;
                fldMenuMagic.unk0 = fldMenuMagic.sel;
                fldMenuMagic.chr = fldMenuMagic.sel;
                fldMenuMagic.help = 0;
            } else if (padPort0[0].trigger & 0x20) {
                if (ItemEggSpellCanCast(MCHR(chr).id, MCHR(chr).lv, fldMenuMagic.cur) == 0) {
                    fldMenuMagic.state = 1;
                    fldMenuMagic.unk30 = 0;
                    fldMenuMagic.unk2C = 0;
                    fldMenuMagic.timer = 0;
                    FldMenuWinOpen(0xB, 0x140, 0x118, 0x244, 0x12C, 0, 8, 0x11);
                    FldMenuPartyPanelOpen(0xC, 0x140, 0x12C);
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            } else {
                if (padPort0[0].repeat & 0x1000) {
                    FldMenuListCursorUp((u32 *)&fldMenuMagic.cur, (u32 *)&top, 9, 4, cnt);
                }
                if (padPort0[0].repeat & 0x4000) {
                    FldMenuListCursorDown((u32 *)&fldMenuMagic.cur, (u32 *)&top, 9, 4, cnt);
                }
                if (padPort0[0].trigger & 0x10) {
                    if (fldMenuMagic.help != 0) {
                        fldMenuMagic.help = 0;
                    } else {
                        fldMenuMagic.help = 1;
                    }
                    fldMenuMagic.last = -1;
                }
                FldMenuCursorSetAt(fldMenuMagic.page + 8, (f32)(u32)((cnt - fldMenuMagic.cur) * 6) + 16.0f,
                              (f32)(u32)(fldMenuMagic.cur * 0x18) + 16.0f);
                FldMenuWinSetActive(fldMenuMagic.page + 8);
                FldMenuWinLrArrowsSet(fldMenuMagic.page + 8, 1, 12.0f);
            }
        }
        break;
    case 1:
        FldMenuPartyPanelUpdate(0xC);
        if (fldMenuMagic.timer == 0) {
            FldMenuPartySelect(0xC, &fldMenuMagic.unk2C, &fldMenuMagic.unk30);
            if (padPort0[0].repeat & 0x20) {
                if (ItemEggSpellCastField(MCHR(chr).id, MCHR(chr).lv, fldMenuMagic.cur,
                                  fldMenuMagic.unk30 + fldMenuMagic.unk2C * 2) == 0) {
                    if (ItemEggSpellCanCast(MCHR(chr).id, MCHR(chr).lv, fldMenuMagic.cur) != 0) {
                        fldMenuMagic.timer = 0x3C;
                    }
                    if ((u8)MCHR(chr).e[fldMenuMagic.cur].kind == 5 ||
                        (u8)MCHR(chr).e[fldMenuMagic.cur].kind == 6) {
                        for (i = 0; i < 4; i++) {
                            FldMenuPartyPanelFlash(i);
                        }
                    } else {
                        FldMenuPartyPanelFlash(fldMenuMagic.unk30 + fldMenuMagic.unk2C * 2);
                    }
                    SndSePlay(0x7D8, 0, 0, 0x1E, 0);
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            } else if (padPort0[0].trigger & 0x40) {
                fldMenuMagic.state = 0;
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                for (i = 0; i < 5; i++) {
                    FldMenuWinClose(i + 0xB, 0, 3);
                }
            }
        } else {
            fldMenuMagic.timer--;
            if (padPort0[0].trigger & 0x20) {
                fldMenuMagic.timer = 0;
            }
            if (padPort0[0].trigger & 0x40) {
                fldMenuMagic.timer = 0;
            }
            if (fldMenuMagic.timer == 0) {
                for (i = 0; i < 5; i++) {
                    FldMenuWinClose(i + 0xB, 0, 3);
                }
                fldMenuMagic.state = 0;
            }
        }
        break;
    }

    FldMenuWinTextClear(fldMenuMagic.page + 8);
    if (party == 0) {
        FldMenuWinTextAdd(fldMenuMagic.page + 8, fldMenuMagic.page, sysGrxsysPac->msg->tbl[16], 0x80, 0x50, 0x14, 0x14, n++,
                      0xF, 3);
    }
    FldMenuWinTextAdd(fldMenuMagic.page + 8, fldMenuMagic.page, sysGrxsysPac->msg->tbl[0], 0x19C, 0xA6, 0x18, 0x14, n++, 0x24,
                  6);
    p0 = FldMenuWinGetSprBuf(fldMenuMagic.page + 8, 0);
    FldMenuWinSetLayer(fldMenuMagic.page + 8, 0, &fldMenuTextures[0], p0, 128.0f);
    p1 = FldMenuWinGetSprBuf(fldMenuMagic.page + 8, 1);
    FldMenuWinSetLayer(fldMenuMagic.page + 8, 1, &fldMenuTextures[4], p1, 128.0f);
    q = (FldMenuQuad *)FldMenuWinGetQuadBuf(fldMenuMagic.page + 8);
    p2 = FldMenuWinGetSprBuf(fldMenuMagic.page + 8, 2);
    FldMenuWinSetLayer(fldMenuMagic.page + 8, 2, &fldMenuTextures[2], p2, 128.0f);
    p3 = FldMenuWinGetSprBuf(fldMenuMagic.page + 8, 3);
    FldMenuWinSetLayer(fldMenuMagic.page + 8, 3, &fldMenuTextures[4], p3, 128.0f);
    if (party != 0) {
        p2->type = 1;
        p2->v[0].r = 0x80;
        p2->v[0].g = 0x80;
        p2->v[0].b = 0x80;
        p2->v[0].a = 0x80;
        p2->v[0].x = 0x100;
        p2->v[0].y = -12;
        p2->v[1].x = 0x11E;
        p2->v[1].y = 8;
        p2->v[0].u = 0xCE;
        p2->v[0].v = 0x6E;
        p2->v[1].u = 0xEC;
        p2->v[1].v = 0x82;
        p2->z = 0;
        p2++;
    }
    FldMenuCursorGetPos(&x, &y);
    if (fldMenuMagic.state == 1) {
        FldMenuLrArrowsUnlock();
    }
    if (party != 0) {
        fldMenuMagic.helpId = MCHR(chr).e[fldMenuMagic.cur].help;
        if (fldMenuMagic.state != 0) {
            fldMenuMagic.helpId = 0;
        }
    } else {
        fldMenuMagic.helpId = 0;
    }
    if (FldMenuWinGetState(fldMenuMagic.page + 8) != 4) {
        fldMenuMagic.helpId = 0;
        fldMenuMagic.last = -1;
    }
    if (fldMenuMagic.helpId != 0) {
        if (fldMenuMagic.last != fldMenuMagic.cur) {
            fldMenuMagic.last = fldMenuMagic.cur;
            FldMenuCursorGetPos(&x, &y);
            if (fldMenuMagic.help != 0) {
                MenuHelpOpen(0, 0, fldMenuMagic.helpId, x, y + 16.0f);
            } else {
                MenuHelpOpen(1, 0, fldMenuMagic.helpId, 32.0f, 348.0f);
            }
        }
    } else {
        fldMenuMagic.last = -1;
        MenuHelpClose();
    }

    if (party != 0) {
        FldMenuWinTextAdd(fldMenuMagic.page + 8, fldMenuMagic.page, sysGrxsysPac->msg->tbl[9], 0x194, -12, 0x12, 0x12, n++, 8,
                      2);
        FldMenuWinTextAdd(fldMenuMagic.page + 8, fldMenuMagic.page, sysGrxsysPac->msg->tbl[9], 0x134, -12, 0x10, 0x12, n++, 7,
                      2);
        for (i = 0; i < cnt; i++) {
            if (i == fldMenuMagic.cur) {
                col = 8;
                if ((fldMenuMagic.offs[k] -= 8) < (cnt - i) * 6) {
                    fldMenuMagic.offs[k] = (cnt - i) * 6;
                }
                h = 0x16;
            } else {
                col = 0;
                if ((fldMenuMagic.offs[k] += 8) >= (cnt - i) * 6 + 0x10) {
                    fldMenuMagic.offs[k] = (cnt - i) * 6 + 0x10;
                }
                h = 0x14;
            }
            ic = MCHR(chr).e[i].icon;
            if (ItemEggSpellCanCast(MCHR(chr).id, MCHR(chr).lv, i) != 0) {
                col = 10;
            }
            FldMenuWinTextAdd(fldMenuMagic.page + 8, fldMenuMagic.page, MCHR(chr).e[i].name,
                          fldMenuMagic.offs[k] + 0x20, i * 0x18 + 8, h, h, n++, -1, col);
            if (MCHR(chr).e[k].kind != 0) {
                FldMenuWinTextAdd(fldMenuMagic.page + 8, fldMenuMagic.page, sysGrxsysPac->msg->tbl[9],
                              fldMenuMagic.offs[k] + 0x114, i * 0x18 + 8, 0x14, 0x14, n++,
                              MCHR(chr).e[k].kind, 0);
            }
            p0 = FldMenuNumberBuild(p0, MCHR(chr).e[i].num, 0, fldMenuMagic.offs[k] + 0xF0, i * 0x18 + 8, 1.0f,
                               1.0f, 0);
            p1->type = 1;
            p1->v[0].r = 0x80;
            p1->v[0].g = 0x80;
            p1->v[0].b = 0x80;
            p1->v[0].a = 0;
            p1->v[0].x = fldMenuMagic.offs[k] + 8;
            p1->v[0].y = i * 0x18 + 8;
            p1->v[1].x = fldMenuMagic.offs[k] + (menuElementIcons[ic].w + 8);
            p1->v[1].y = i * 0x18 + (menuElementIcons[ic].h + 8);
            p1->v[0].u = menuElementIcons[ic].u0;
            p1->v[0].v = menuElementIcons[ic].v0;
            p1->v[1].u = menuElementIcons[ic].u1;
            p1->v[1].v = menuElementIcons[ic].v1;
            p1->z = 0;
            for (j = 0; j < 5; j++) {
                if (MCHR(chr).e[i].slot[j] != 0) {
                    ic = MCHR(chr).e[i].slot[j] - 1;
                    p3->type = 1;
                    p3->v[0].r = 0x80;
                    p3->v[0].g = 0x80;
                    p3->v[0].b = 0x80;
                    p3->v[0].a = 0;
                    p3->v[0].x = (s16)(fldMenuMagic.offs[k] + 0x152) + j * 26;
                    p3->v[0].y = fldMenuJunctionIcons[ic].h + (i * 0x18 - (u16)(fldMenuMagic.bars[j] - 8));
                    xx = j * 26 + 0x120;
                    p3->v[1].x = fldMenuMagic.offs[k] + fldMenuJunctionIcons[ic].w + xx + 0x32;
                    p3->v[1].y = fldMenuMagic.bars[j] + (fldMenuJunctionIcons[ic].h + (i * 0x18 - (u16)(fldMenuMagic.bars[j] - 8)));
                    p3->v[0].u = fldMenuJunctionIcons[ic].u0;
                    p3->v[0].v = fldMenuJunctionIcons[ic].v0;
                    p3->v[1].u = fldMenuJunctionIcons[ic].u1;
                    p3->v[1].v = fldMenuJunctionIcons[ic].v1;
                    p3->z = 0;
                    p3++;
                }
            }
            q = FldMenuBarBuild(q, fldMenuMagic.offs[k] + 4, i * 0x18 + 0x14, 0x1D6, 0xC, MCHR(chr).e[i].col);
            p1++;
            k++;
        }
    }
    p0->type = 0;
    p1->type = 0;
    q->type = 0;
    p2->type = 0;
    p3->type = 0;

    FldMenuWinTextClear(0xA);
    p0 = FldMenuWinGetSprBuf(0xA, 0);
    FldMenuWinSetLayer(0xA, 0, &fldMenuTextures[6], p0, 128.0f);
    p1 = FldMenuWinGetSprBuf(0xA, 1);
    FldMenuWinSetLayer(0xA, 1, &fldMenuTextures[1], p1, 128.0f);
    q = (FldMenuQuad *)FldMenuWinGetQuadBuf(0xA);
    p2 = FldMenuWinGetSprBuf(0xA, 2);
    FldMenuWinSetLayer(0xA, 2, &fldMenuTextures[2], p2, 128.0f);
    p3 = FldMenuWinGetSprBuf(0xA, 3);
    FldMenuWinSetLayer(0xA, 3, &fldMenuTextures[0], p3, 128.0f);
    if (party != 0) {
        p3 = FldMenuHpFractionBuild(p3, MCHR(chr).x, MCHR(chr).y, 0xE0, 0x40);
        p2->type = 1;
        p2->v[0].r = 0x80;
        p2->v[0].g = 0x80;
        p2->v[0].b = 0x80;
        p2->v[0].x = 0xA8;
        p2->v[0].y = 0x40;
        p2->v[1].x = 0xC6;
        p2->v[1].y = 0x54;
        p2->v[0].u = 0xCE;
        p2->v[0].a = 0x80;
        p2->v[0].v = 0x6E;
        p2->v[1].u = 0xEC;
        p2->v[1].v = 0x82;
        p2->z = 0;
        p2++;
        FldMenuBarBuild(q, 0x70, 0x30, 0xE0, 0xC, 0);
    }
    q = FldMenuWinGetSprBuf(0xA, 4);
    FldMenuWinSetLayer(0xA, 4, &fldMenuTextures[4], q, 128.0f);
    if (++fldMenuMagic.blink >= 2) {
        fldMenuMagic.blink = 0;
    }
    for (i = 0; i < party; i++) {
        p1->type = 1;
        if (i == chr) {
            p1->v[0].a = 0;
            p1->v[0].r = 0x80;
            p1->v[0].g = 0x80;
            p1->v[0].b = 0x80;
            if (fldMenuMagic.blink == 0) {
                if (fldMenuMagic.bdir != 0) {
                    if (fldMenuMagic.bob[chr] >= -3) {
                        fldMenuMagic.bob[chr]--;
                    } else {
                        fldMenuMagic.bdir = 0;
                    }
                } else {
                    if (fldMenuMagic.bob[chr] < 2) {
                        fldMenuMagic.bob[chr]++;
                    } else {
                        fldMenuMagic.bdir = 1;
                    }
                }
            }
        } else {
            p1->v[0].r = 0x80 - (fldMenuMagic.bob[i] + 4) * 9;
            p1->v[0].g = 0x80 - (fldMenuMagic.bob[i] + 4) * 9;
            p1->v[0].b = 0x80 - (fldMenuMagic.bob[i] + 4) * 9;
            p1->v[0].a = 0;
            if (fldMenuMagic.blink == 0) {
                if (fldMenuMagic.bob[i] < 4) {
                    fldMenuMagic.bob[i]++;
                }
            }
        }
        p1->v[0].x = i * 20 + (s16)(fldMenuMagic.bob[i] / 2 + 4);
        p1->v[0].y = fldMenuMagic.bob[i] + 8;
        p1->v[1].x = (s16)(i * 20 + (menuItemIcons[MCHR(i).icon].w - fldMenuMagic.bob[i])) +
                 (s16)(fldMenuMagic.bob[i] / 2 + 4);
        p1->v[1].y = fldMenuMagic.bob[i] + (s16)(menuItemIcons[MCHR(i).icon].h - fldMenuMagic.bob[i] + 8);
        p1->z = 0;
        p1->v[0].u = menuItemIcons[MCHR(i).icon].u0;
        p1->v[0].v = menuItemIcons[MCHR(i).icon].v0;
        p1->v[1].u = menuItemIcons[MCHR(i).icon].u1;
        p1->v[1].v = menuItemIcons[MCHR(i).icon].v1;
        p1++;
        for (j = 0; j < MCHR(i).orbs; j++) {
            VuSinCosDeg(&sn, &cs, j * 110.0f + fldMenuMagic.angle);
            q->type = 1;
            if (MCHR(i).orbs == 1) {
                q->v[0].a = 0;
                q->v[0].r = 0x80;
                q->v[0].g = 0x80;
                q->v[0].b = 0x80;
            } else {
                q->v[0].r = 0xA0;
                q->v[0].g = 0x80;
                q->v[0].b = 0x80;
                q->v[0].a = 0;
            }
            q->v[0].x = (s16)(i * 20 + (s32)(cs * 7.0f)) + (s16)(fldMenuMagic.bob[i] / 2 + 4);
            q->v[0].y = fldMenuMagic.bob[i] + (s16)((s32)(sn * 5.0f) + 8);
            q->v[1].x = fldMenuMagic.bob[i] / 2 + (s16)(i * 20 + (s32)(cs * 7.0f) + 0xE);
            q->v[1].y = (s16)(fldMenuMagic.bob[i] + 0x12) + (s32)(sn * 5.0f);
            q->v[0].u = sprStarIconRects[0].u0;
            q->v[0].v = sprStarIconRects[0].v0;
            q->v[1].u = sprStarIconRects[0].u1;
            q->v[1].v = sprStarIconRects[0].v1;
            q->z = 0;
            q++;
        }
    }
    p1->type = 0;
    q->type = 0;
    if (party != 0) {
        if (MCHR(chr).id >= 0) {
            FldMenuWinTextAdd(0xA, fldMenuMagic.page, MCHR(chr).name, 0x80, 0x20, 0x14, 0x14, n++, -1, 8);
            FldMenuWinTextAdd(0xA, fldMenuMagic.page, sysGrxsysPac->msg->tbl[2], 0x3A, 0x40, 0x10, 0x10, n,
                          MCHR(chr).id, 1);
            p0->type = 1;
            p0->v[0].r = 0x80;
            p0->v[0].g = 0x80;
            p0->v[0].b = 0x80;
            p0->v[0].a = 0;
            p0->v[0].x = 4;
            p0->v[0].y = 0x20;
            p0->v[1].x = 0x3A;
            p0->v[1].y = 0x56;
            p0->z = 0;
            if (MCHR(chr).id >= 4) {
                p0->v[0].u = (MCHR(chr).id - 4) * 0x36;
                p0->v[0].v = 0x36;
                p0->v[1].u = (MCHR(chr).id - 4) * 0x36 + 0x36;
                p0->v[1].v = 0x6C;
            } else {
                p0->v[0].u = MCHR(chr).id * 0x36;
                p0->v[0].v = 0;
                p0->v[1].u = MCHR(chr).id * 0x36 + 0x36;
                p0->v[1].v = 0x36;
            }
            p0++;
        }
    }
    p0->type = 0;
    p1->type = 0;
    q->type = 0;
    p2->type = 0;
    p3->type = 0;

    if (FldMenuWinGetState(0xB) != 0) {
        FldMenuWinTextClear(0xB);
        p0 = FldMenuWinGetSprBuf(0xB, 0);
        FldMenuWinSetLayer(0xB, 0, &fldMenuTextures[0], p0, 128.0f);
        p1 = FldMenuWinGetSprBuf(0xB, 1);
        FldMenuWinSetLayer(0xB, 1, &fldMenuTextures[4], p1, 128.0f);
        q = (FldMenuQuad *)FldMenuWinGetQuadBuf(0xB);
        pg = page2;
        p2 = FldMenuWinGetSprBuf(0xB, 2);
        FldMenuWinSetLayer(0xB, 2, &fldMenuTextures[2], p2, 128.0f);
        p3 = FldMenuWinGetSprBuf(0xB, 3);
        FldMenuWinSetLayer(0xB, 3, &fldMenuTextures[4], p3, 128.0f);
        p2->v[0].r = 0x80;
        p2->v[0].g = 0x80;
        p2->v[0].b = 0x80;
        p2->v[0].a = 0x80;
        p2->type = 1;
        p2->z = 0;
        p2->v[0].x = 0x1A4;
        p2->v[0].y = 8;
        p2->v[1].x = 0x1C2;
        p2->v[1].y = 0x1C;
        p2->v[0].u = 0xCE;
        p2->v[0].v = 0x6E;
        p2->v[1].u = 0xEC;
        p2->v[1].v = 0x82;
        p2++;
        p0 = FldMenuNumberBuild(p0, MCHR(chr).e[fldMenuMagic.cur].num, 0, 0x1CA, 8, 1.0f, 1.0f, 0);
        q = FldMenuBarBuild(q, 0x20, 0x18, 0x1E0, 0xC, 0);
        FldMenuWinTextAdd(0xB, pg, MCHR(chr).e[fldMenuMagic.cur].name, 0x50, 6, 0x16, 0x16, 0, -1, 8);
        FldMenuWinTextAdd(0xB, pg, sysGrxsysPac->msg->tbl[0], 0x60, 0x22, 0x18, 0x18, 1, 0x18, 3);
        ic = MCHR(chr).e[fldMenuMagic.cur].icon;
        p1->type = 1;
        p1->v[0].r = 0x80;
        p1->v[0].g = 0x80;
        p1->v[0].b = 0x80;
        p1->v[0].a = 0;
        p1->v[0].x = 0x30;
        p1->v[0].y = 8;
        p1->v[1].x = menuElementIcons[ic].w + 0x30;
        p1->v[1].y = menuElementIcons[ic].h + 8;
        p1->v[0].u = menuElementIcons[ic].u0;
        p1->v[0].v = menuElementIcons[ic].v0;
        p1->v[1].u = menuElementIcons[ic].u1;
        p1->v[1].v = menuElementIcons[ic].v1;
        p1->z = 0;
        p1++;
        for (j = 0; j < 5; j++) {
            if (MCHR(chr).e[fldMenuMagic.cur].slot[j] != 0) {
                ic = MCHR(chr).e[fldMenuMagic.cur].slot[j] - 1;
                p3->type = 1;
                p3->v[0].r = 0x80;
                p3->v[0].g = 0x80;
                p3->v[0].b = 0x80;
                p3->v[0].a = 0;
                p3->v[0].x = j * 0x18 + 0x12C;
                p3->v[0].y = 8;
                p3->v[1].x = j * 0x18 + (fldMenuJunctionIcons[ic].w + 0x12C);
                p3->v[1].y = fldMenuJunctionIcons[ic].h + 8;
                p3->v[0].u = fldMenuJunctionIcons[ic].u0;
                p3->v[0].v = fldMenuJunctionIcons[ic].v0;
                p3->v[1].u = fldMenuJunctionIcons[ic].u1;
                p3->v[1].v = fldMenuJunctionIcons[ic].v1;
                p3->z = 0;
                p3++;
            }
        }
        p0->type = 0;
        p1->type = 0;
        q->type = 0;
        p2->type = 0;
        p3->type = 0;
    }
    return 7;
}
