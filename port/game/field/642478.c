/* Port-only C for FldMenuJunctionUpdate (0x00642478), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "sys.h"
#include "field/menu.h"
#include "field/n23.h"
#include "field/n24.h"
#include "field/state.h"

/*
 * Based on the decompilation rounds' near miss (3661 of 3661 instructions;
 * the differences are stack slots, two swapped registers and scheduling).
 */

/* fldMenuJunction (FldMenuJunction in field/n24.h, 0x60) as this screen reads it. */
typedef struct FldMenuJunctionView {
    /* 0x00 */ s8 state;
    /* 0x01 */ u8 pad1[0xB];
    /* 0x0C */ u32 page;
    /* 0x10 */ u32 cursor;
    /* 0x14 */ u32 confirm;
    /* 0x18 */ s32 egg0;
    /* 0x1C */ s32 egg1;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u32 timer;
    /* 0x28 */ u32 warpTarget;
    /* 0x2C */ u8 alphaA;
    /* 0x2D */ u8 alphaB;
    /* 0x2E */ u8 slideA;
    /* 0x2F */ u8 slideB;
    /* 0x30 */ u8 blink;
    /* 0x31 */ u8 pad31;
    /* 0x32 */ u16 frames;
    /* 0x34 */ u8 fade;
    /* 0x35 */ u8 tick;
    /* 0x36 */ u8 pad36[2];
    /* 0x38 */ f32 flyT;
    /* 0x3C */ s16 ang;
    /* 0x3E */ u8 wobTick;
    /* 0x3F */ u8 wobDir;
    /* 0x40 */ s8 wob[0x20];
} FldMenuJunctionView;

#define J (*(FldMenuJunctionView *)&fldMenuJunction)

/* Mana egg with its spells (ItemEggSpellRec, 0x98). */
typedef struct JuncEggSpell {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ s8 icon;
    /* 0x02 */ u8 pad2[4];
    /* 0x06 */ s8 ids[5];
    /* 0x0B */ u8 padB;
    /* 0x0C */ s32 name;
    /* 0x10 */ u8 pad10[4];
} JuncEggSpell;

typedef struct JuncEgg {
    /* 0x00 */ s16 itemId;
    /* 0x02 */ u8 pad2;
    /* 0x03 */ s8 icon;
    /* 0x04 */ s8 nSpells;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 name;
    /* 0x0C */ u8 padC[6];
    /* 0x12 */ s16 mp;
    /* 0x14 */ s16 rarity;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ JuncEggSpell spells[6];
    /* 0x90 */ u8 pad90[8];
} JuncEgg;

typedef struct JuncItemWork {
    /* 0x0000 */ u8 pad0[0x4A8C];
    /* 0x4A8C */ JuncEgg eggSpells[24];
    /* 0x58CC */ u8 pad58CC[0x1570];
    /* 0x6E3C */ JuncEgg preview;
} JuncItemWork;

#define PREVIEW (&((JuncItemWork *)itemWork)->preview)
#define SPELLS (((JuncItemWork *)itemWork)->eggSpells)

extern f32 fldMenuJunctionMeshX[5][5];
extern s32 fldMenuJunctionWarpAmp;
extern const SprRect ipIconPartyRects[];

void VuSinCosDeg(f32 *, f32 *, f32);
FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuJunctionUpdate(void) {
    f32 sn;
    f32 cs;
    s32 count;
    s32 sel;
    FldMenuQuad *s20;
    FldMenuQuad *s24;
    s32 cnt;
    s32 n;
    s32 i;
    s32 k;
    s32 icon;
    s32 off;
    s16 h;
    s32 w;
    s32 n5;
    FldMenuQuad *s4;
    FldMenuQuad *s5;
    FldMenuQuad *s7;
    FldMenuQuad *q;
    s32 amp;

    cnt = 0;
    n = 0;
    count = itemWork->counts[1];
    if ((s8)++J.tick < 0) {
        J.tick = 0;
    }
    if ((J.ang += 5) >= 360) {
        J.ang -= 360;
    }
    if (++J.wobTick >= 2) {
        J.wobTick = 0;
    }
    switch (J.state) {
    case 0:
        J.slideA = 0;
        J.slideB = 0;
        J.alphaA = 0x80;
        J.alphaB = 0x80;
        if (count >= 2) {
            if (padPort0[0].repeat & 0x1000) {
                if (J.page != 0) {
                    SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                    J.page = 0;
                }
                break;
            }
            if (padPort0[0].repeat & 0x4000) {
                if (J.page == 0) {
                    if (count >= 13) {
                        J.page = 1;
                        if (J.cursor >= count - 12) {
                            J.cursor = count - 13;
                        }
                    }
                    SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                }
                break;
            }
            if (padPort0[0].repeat & 0x8000) {
                if (J.cursor != 0) {
                    J.cursor--;
                } else if (count >= 13) {
                    if (J.page) {
                        J.cursor = 11;
                    } else {
                        J.cursor = count - 13;
                    }
                    J.page ^= 1;
                } else {
                    J.page = 0;
                    J.cursor = count - 1;
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                break;
            }
            if (padPort0[0].repeat & 0x2000) {
                if (J.page == 0) {
                    if (count >= 13) {
                        if (J.cursor < 11) {
                            J.cursor++;
                        } else {
                            J.page = 1;
                            J.cursor = 0;
                        }
                    } else {
                        if (J.cursor < count - 1) {
                            J.cursor++;
                        } else {
                            J.page = 0;
                            J.cursor = 0;
                        }
                    }
                } else {
                    if (J.cursor < count - 13) {
                        J.cursor++;
                    } else {
                        J.page = 0;
                        J.cursor = 0;
                    }
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                break;
            }
        }
        if (padPort0[0].repeat & 0x40) {
            if (FldMenuWinGetState(0xB) == 4) {
                FldMenuWinClose(0xC, 0, 3);
                FldMenuWinClose(0xB, 0, 1);
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            } else if (FldMenuWinGetState(0xA) == 4) {
                FldMenuWinClose(0xA, 0, 0);
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            } else if (FldMenuWinGetState(0xA) == 0) {
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                FldMenuClose();
            }
        } else if (count >= 2 && (padPort0[0].repeat & 0x20)) {
            if (FldMenuWinGetState(0xA) == 0) {
                FldMenuWinOpen(0xA, 0xA8, 0xD4, 4, 4, 0, 0, 0x400000);
                J.egg0 = J.cursor + J.page * 12 + 1;
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            } else if (FldMenuWinGetState(0xA) == 4 && FldMenuWinGetState(0xB) == 0) {
                s32 t = J.cursor + J.page * 12;
                if (J.egg0 != t + 1) {
                    ItemEggFusePreview(itemWork->eggs[t].index, itemWork->eggs[J.egg0 - 1].index);
                    if ((s8)itemWork->unk6ECC == 0) {
                        FldMenuWinOpen(0xB, 0x168, 0xD4, 4, 4, 0, 1, 0x400000);
                        FldMenuWinOpen(0xC, 0xF8, 0x10E, 4, 4, 0, 3, 0x400000);
                        J.unk20 = 1;
                        J.state = 1;
                        J.egg1 = J.cursor + J.page * 12 + 1;
                        FldMenuWinOpen(0xD, 0x20, 0x50, 4, 4, 0, 0, 0x400000);
                        J.confirm = 1;
                        SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                        FldMenuJunctionScatterInit();
                    } else {
                        SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                    }
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            }
        }
        break;
    case 1:
        if ((padPort0[0].repeat & 0x4000) && J.confirm == 0) {
            J.confirm = 1;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if ((padPort0[0].repeat & 0x1000) && J.confirm != 0) {
            J.confirm = 0;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (padPort0[0].repeat & 0x40) {
            FldMenuWinClose(0xD, 0, 0);
            FldMenuWinClose(0xC, 0, 3);
            FldMenuWinClose(0xB, 0, 1);
            J.state = 0;
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
        } else if (padPort0[0].repeat & 0x20) {
            if (J.confirm == 0) {
                if ((u32)itemWork->gold >= (u32)itemWork->unk6ED0) {
                    PtySetGold(itemWork->gold - itemWork->unk6ED0);
                    J.state = 2;
                    FldMenuWinClose(0xD, 0, 0);
                    FldMenuWinClose(0xC, 0, 3);
                    FldMenuWinClose(9, 0, 2);
                    FldMenuWinOpen(0xE, 0x10E, 0x40, 4, 4, 0, 2, 0x400000);
                    FldMenuJunctionMeshInit();
                    SndSePlay(0x7D9, 0, 0, 0x1E, 0);
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            } else {
                FldMenuWinClose(0xD, 0, 0);
                FldMenuWinClose(0xC, 0, 3);
                FldMenuWinClose(0xB, 0, 1);
                J.state = 0;
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            }
        }
        break;
    case 2:
        if (J.alphaA != 0x20) {
            if (--J.alphaA == 0x20) {
                SndSePlay(0x7E0, 0, 0, 0x1E, 0);
            }
        } else if (J.slideA != 0x18) {
            if (++J.slideA == 0x18) {
                FldMenuWinClose(0xA, 0, 0);
                SndSePlay(0x7D5, 0, 0, 0x1E, 0);
            }
        } else if (J.alphaB != 0x20) {
            if (--J.alphaB == 0x20) {
                SndSePlay(0x7E0, 0, 0, 0x1E, 0);
            }
        } else if (J.slideB != 0x18) {
            if (++J.slideB == 0x18) {
                FldMenuWinClose(0xB, 0, 1);
                SndSePlay(0x7D5, 0, 0, 0x1E, 0);
            }
        } else {
            J.state = 3;
            SndSePlay(0x801, 0, 0, 0x1E, 0);
        }
        break;
    case 3:
        if (++J.timer == 1) {
            J.warpTarget = 20;
        }
        if (J.timer == 20) {
            SndSePlay(0x802, 0, 0, 0x1E, 0);
            GphFadeStart(&fldFades[1], 0, 255.0f, 255.0f, 255.0f, 64.0f, 4.0f);
        }
        if (J.timer == 26) {
            GphFadeStart(&fldFades[1], 0, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f);
        }
        if (J.timer == 20) {
            J.warpTarget = 50;
        }
        if (J.timer == 100) {
            J.warpTarget = 150;
        }
        if (J.timer == 200) {
            J.warpTarget = 400;
        }
        if (J.timer == 250) {
            J.warpTarget = 150;
            GphFadeStart(&fldFades[1], 0, 255.0f, 255.0f, 255.0f, 64.0f, 4.0f);
            SndSePlay(0x802, 0, 0, 0x1E, 0);
        }
        if (J.timer == 256) {
            GphFadeStart(&fldFades[1], 0, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f);
        }
        if (J.timer == 300) {
            J.warpTarget = 40;
        }
        if (J.timer == 350) {
            J.warpTarget = 20;
        }
        if (J.timer == 400) {
            J.warpTarget = 0;
        }
        if (J.timer == 430) {
            FldMenuWinClose(0xE, 0, 2);
            FldMenuWinOpen(0xD, 0x40, 0x100, 4, 0x160, 0, 0, 0x400000);
            SndSePlay(0x7D5, 0, 0, 0x1E, 0);
            J.frames = 0;
            J.state = 4;
            J.flyT = 0.0f;
            SndSePlay(0x7FB, 0, 0, 0x1E, 0);
        }
        if (fldMenuJunctionWarpAmp < J.warpTarget) {
            fldMenuJunctionWarpAmp += 4;
        } else if (fldMenuJunctionWarpAmp > 40) {
            fldMenuJunctionWarpAmp -= 4;
        } else if (fldMenuJunctionWarpAmp != 0) {
            fldMenuJunctionWarpAmp--;
        }
        break;
    case 4:
        J.frames++;
        if ((padPort0[0].trigger & 0x20) && FldMenuWinGetState(0xD) == 4) {
            FldMenuWinClose(0xD, 0, 0);
            SndSePlay(0x7D5, 0, 0, 0x1E, 0);
        }
        if (FldMenuWinGetState(0xD) == 0) {
            J.state = 0;
            FldMenuWinOpen(9, 0x20, 0xD4, 4, 0x160, 0, 1, 0x400000);
            ItemEggFuseCommit(itemWork->eggs[J.egg0 - 1].index, itemWork->eggs[J.egg1 - 1].index);
            J.egg0 = 0;
            J.egg1 = 0;
            J.unk20 = 0;
            J.page = 0;
            J.cursor = 0;
        }
        break;
    }
    sel = J.cursor + J.page * 12;
    FldMenuWinTextClear(9);
    FldMenuWinTextClear(0xA);
    FldMenuWinTextClear(0xB);
    FldMenuWinTextClear(0xC);
    FldMenuWinTextClear(0xE);
    FldMenuWinTextClear(0xD);
    if (count < 2) {
        FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[16], 0xB2, 0x94, 0x14, 0x14, n++, 0xC, 2);
        FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[16], 0xBC, 0xA8, 0x14, 0x14, n++, 0xD, 2);
    }
    if (++J.blink >= 21) {
        J.blink = 0;
    }
    if (J.blink < 14) {
        switch (J.state) {
        case 2:
            FldMenuWinTextAdd(0xE, 0, sysGrxsysPac->msg->tbl[16], 0, 0, 0x20, 0x20, n++, 7, 1);
            break;
        case 3:
        case 4:
            FldMenuWinTextAdd(0xE, 0, sysGrxsysPac->msg->tbl[16], 0x10, 0, 0x20, 0x20, n++, 8, 2);
            break;
        }
    }
    if (J.state == 4) {
        FldMenuBarBuild((FldMenuQuad *)FldMenuWinGetQuadBuf(0xD), 0x10, 0x2C, 0x180, 0xC, 0);
        s4 = FldMenuWinGetSprBuf(0xD, 1);
        FldMenuWinSetLayer(0xD, 1, &fldMenuTextures[1], s4, 128.0f);
        s5 = FldMenuWinGetSprBuf(0xD, 2);
        FldMenuWinSetLayer(0xD, 2, &fldMenuTextures[4], s5, 128.0f);
        q = FldMenuWinGetSprBuf(0xD, 3);
        FldMenuWinSetLayer(0xD, 3, &fldMenuTextures[4], q, 128.0f);
        q = FldMenuWinGetSprBuf(0xD, 4);
        FldMenuWinSetLayer(0xD, 4, &fldMenuTextures[2], q, 128.0f);
        q = FldMenuWinGetSprBuf(0xD, 5);
        FldMenuWinSetLayer(0xD, 5, &fldMenuTextures[0], q, 128.0f);
        icon = PREVIEW->icon;
        s4->type = 1;
        s4->v[0].x = 0x20;
        s4->v[0].y = 0x1C;
        s4->v[0].r = 0x80;
        s4->v[1].x = menuItemIcons[icon].w + 0x20;
        s4->v[1].y = menuItemIcons[icon].h + 0x1C;
        s4->v[0].u = menuItemIcons[icon].u0;
        s4->v[0].v = menuItemIcons[icon].v0;
        s4->v[1].u = menuItemIcons[icon].u1;
        s4->v[0].g = 0x80;
        s4->v[0].b = 0x80;
        s4->v[0].a = 0;
        s4->z = 0;
        s4->v[1].v = menuItemIcons[icon].v1;
        for (i = 0; i < PREVIEW->rarity; i++) {
            VuSinCosDeg(&sn, &cs, i * 110.0f + J.ang);
            s5->type = 1;
            if (PREVIEW->rarity == 1) {
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
            s5->v[0].x = 0x20 + (s32)(cs * 7.0f);
            s5->v[0].u = sprStarIconRects[0].u0;
            s5->v[0].v = sprStarIconRects[0].v0;
            s5->v[1].x = 0x20 + (s32)(cs * 7.0f) + 10;
            s5->v[0].y = 0x1C + (s32)(sn * 5.0f);
            s5->v[1].y = 0x1C + (s32)(sn * 5.0f) + 10;
            s5->v[1].u = sprStarIconRects[0].u1;
            s5->v[1].v = sprStarIconRects[0].v1;
            s5->z = 0;
            s5++;
        }
        FldMenuWinTextAdd(0xD, 0, PREVIEW->name, 0x3C, 0x1C, 0x14, 0x14, n++, -1, 2);
        FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], TxtStrGetWidth((u8 *)PREVIEW->name, 0x14) + 0x3C, 0x1C, 0x14, 0x14, n++, 9, 1);
        for (i = 0; i < PREVIEW->nSpells; i++) {
            if (PREVIEW->spells[i].ids[0] + PREVIEW->spells[i].ids[1] + PREVIEW->spells[i].ids[2] + PREVIEW->spells[i].ids[3] + PREVIEW->spells[i].ids[4]) {
                FldMenuWinTextAdd(0xD, 0, PREVIEW->spells[i].name, 0x80, i * 0x18 + 0x40, 0x14, 0x14, n++, -1, 8);
            } else {
                FldMenuWinTextAdd(0xD, 0, PREVIEW->spells[i].name, 0x80, i * 0x18 + 0x40, 0x14, 0x14, n++, -1, 2);
            }
            icon = PREVIEW->spells[i].icon;
            s5->v[0].r = 0x80;
            s5->v[0].g = 0x80;
            s5->v[0].b = 0x80;
            s5->type = 1;
            s5->v[0].a = 0;
            s5->v[0].x = 0x60;
            s5->v[0].y = i * 0x18 + 0x40;
            s5->v[1].x = 0x60 + menuElementIcons[icon].w;
            s5->v[1].y = i * 0x18 + 0x40 + menuElementIcons[icon].h;
            s5->v[0].u = menuElementIcons[icon].u0;
            s5->v[0].v = menuElementIcons[icon].v0;
            s5->v[1].u = menuElementIcons[icon].u1;
            s5->v[1].v = menuElementIcons[icon].v1;
            s5->z = 0;
            s5++;
        }
        s5->type = 0;
        cnt = FldMenuJunctionScatterBuild(0);
    } else {
        FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], 0x20, 0x1C, 0x14, 0x14, n++, 0xE, 2);
        FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], 0x10, 0x3A, 0x14, 0x14, n++, 3, 1);
        FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], 0x18, 0x4E, 0x14, 0x14, n++, 4, 1);
        FldMenuBarBuild((FldMenuQuad *)FldMenuWinGetQuadBuf(0xD), 0x18, 0x2C, 0x12C, 0xC, 0);
        s4 = FldMenuWinGetSprBuf(0xD, 0);
        FldMenuWinSetLayer(0xD, 0, &fldMenuTextures[0], s4, 128.0f);
        FldMenuNumberBuild(s4, itemWork->unk6ED0, 1, 0xDC, 0x1C, 1.0f, 1.0f, 0);
        FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[0], 0xDC, 0x22, 0x10, 0x10, n++, 0x34, 2);
        if (J.confirm == 0) {
            FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], 0x190, 0x3A, 0x14, 0x14, n++, 5, 8);
            FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], 0x190, 0x4E, 0x14, 0x14, n++, 6, 0);
        } else {
            FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], 0x190, 0x3A, 0x14, 0x14, n++, 5, 0);
            FldMenuWinTextAdd(0xD, 0, sysGrxsysPac->msg->tbl[16], 0x190, 0x4E, 0x14, 0x14, n++, 6, 8);
        }
    }
    FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[16], 0, -8, 0x20, 0x20, n++, 0, 1);
    s4 = FldMenuWinGetSprBuf(9, 0);
    FldMenuWinSetLayer(9, 0, &fldMenuTextures[0], s4, 128.0f);
    s5 = (FldMenuQuad *)FldMenuWinGetQuadBuf(9);
    FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[13], 0x184, 0x46, 0x14, 0x14, n++, 5, 1);
    FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[0], 0x23A, 0x4C, 0x10, 0x10, n++, 0x34, 2);
    FldMenuNumberBuild(s4, itemWork->gold, 1, 0x23A, 0x46, 1.0f, 1.0f, 0);
    s5 = FldMenuBarBuild(s5, 0x184, 0x56, 0xC8, 0xC, 0xA);
    if (FldMenuWinGetState(0xA)) {
        FldMenuWinTextAdd(9, 0, itemWork->eggs[J.egg0 - 1].name, 0x80, 0, 0x14, 0x14, n++, -1, 3);
        w = TxtStrGetWidth((u8 *)itemWork->eggs[J.egg0 - 1].name, 0x14);
        FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[16], w + 0x8C, 0, 0x14, 0x14, n++, 0xB, 2);
        FldMenuWinTextAdd(9, 0, itemWork->eggs[sel].name, w + 0xA8, 0, 0x14, 0x14, n++, -1, 8);
    } else if (count >= 2) {
        FldMenuWinTextAdd(9, 0, itemWork->eggs[sel].name, 0x80, 0, 0x14, 0x14, n++, -1, 8);
    }
    if (count >= 2) {
        FldMenuBarBuild(s5, 0x6C, 0x10, 0x180, 0xC, itemWork->eggs[sel].useClass);
    }
    s4 = FldMenuWinGetSprBuf(9, 1);
    FldMenuWinSetLayer(9, 1, &fldMenuTextures[1], s4, 128.0f);
    s5 = FldMenuWinGetSprBuf(9, 2);
    FldMenuWinSetLayer(9, 2, &fldMenuTextures[4], s5, 128.0f);
    s7 = FldMenuWinGetSprBuf(9, 3);
    FldMenuWinSetLayer(9, 3, &fldMenuTextures[5], s7, 128.0f);
    for (i = 0; i < count; i++) {
        icon = (s8)itemWork->eggs[i].icon;
        s4->type = 1;
        if (i == sel) {
            s4->v[0].a = 0;
            s4->v[0].r = 0x80;
            s4->v[0].g = 0x80;
            s4->v[0].b = 0x80;
            if (J.wobTick == 0) {
                if (J.wobDir != 0) {
                    if (J.wob[i] >= -5) {
                        J.wob[i]--;
                    }
                } else if (J.wob[i] >= 0) {
                    J.wobDir = 1;
                } else {
                    J.wob[i]++;
                }
            }
        } else {
            s4->v[0].a = 0;
            s4->v[0].r = 0x40;
            s4->v[0].g = 0x40;
            s4->v[0].b = 0x40;
            if (J.wobTick == 0) {
                if (J.wob[i] <= 0) {
                    J.wob[i]++;
                }
            }
        }
        off = 0;
        h = menuItemIcons[icon].h;
        if (FldMenuWinGetState(0xA) != 0 && FldMenuWinGetState(0xA) < 5 && i == J.egg0 - 1) {
            off = 10;
            h = h / 2;
        }
        if (FldMenuWinGetState(0xB) != 0 && FldMenuWinGetState(0xB) < 5 && i == J.egg1 - 1) {
            off = 10;
            h = (s16)menuItemIcons[icon].h / 2;
        }
        if (i >= 12) {
            s4->v[0].y = off + 0x36;
            s4->v[1].y = off + 0x36 + h;
            s4->v[0].x = (i - 12) * 30 + 0x20;
            s4->z = 0;
            s4->v[1].x = (i - 12) * 30 + 0x20 + menuItemIcons[icon].w;
        } else {
            s4->v[0].y = off + 0x18;
            s4->z = 0;
            s4->v[0].x = i * 30 + 0x20;
            s4->v[1].x = i * 30 + 0x20 + menuItemIcons[icon].w;
            s4->v[1].y = off + 0x18 + h;
        }
        s4->v[0].u = menuItemIcons[icon].u0;
        s4->v[0].v = menuItemIcons[icon].v0;
        s4->v[1].u = menuItemIcons[icon].u1;
        s4->v[1].v = menuItemIcons[icon].v1;
        s4++;
        for (k = 0; k < itemWork->eggs[i].rarity; k++) {
            VuSinCosDeg(&sn, &cs, k * 110.0f + J.ang);
            s5->type = 1;
            if (itemWork->eggs[i].rarity == 1) {
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
            if (i >= 12) {
                s5->z = 0;
                s5->v[1].x = (i - 12) * 30 + 0x20 + (s32)(cs * 7.0f) + 10;
                s5->v[0].x = (i - 12) * 30 + 0x20 + (s32)(cs * 7.0f);
                s5->v[0].y = off + 0x36 + (s32)(sn * 5.0f);
                s5->v[1].y = off + 0x36 + (s32)(sn * 5.0f) + 10;
            } else {
                s5->z = 0;
                s5->v[1].x = i * 30 + 0x20 + (s32)(cs * 7.0f) + 10;
                s5->v[0].x = i * 30 + 0x20 + (s32)(cs * 7.0f);
                s5->v[0].y = off + 0x18 + (s32)(sn * 5.0f);
                s5->v[1].y = off + 0x18 + (s32)(sn * 5.0f) + 10;
            }
            s5->v[0].u = sprStarIconRects[0].u0;
            s5->v[0].v = sprStarIconRects[0].v0;
            s5->v[1].u = sprStarIconRects[0].u1;
            s5->v[1].v = sprStarIconRects[0].v1;
            s5++;
        }
        if (itemWork->eggs[i].owner >= 0) {
            s7->type = 1;
            s7->v[0].r = 0x80;
            s7->v[0].g = 0x80;
            s7->v[0].b = 0x80;
            s7->v[0].a = 0x80;
            if (i >= 12) {
                s7->v[0].x = (i - 12) * 30 + 0x2A + J.wob[i] / 2;
                s7->v[0].y = 0x40 + J.wob[i] / 2;
                s7->v[1].x = (i - 12) * 30 + 0x2A + J.wob[i] / 2 + 0x10 - J.wob[i];
                s7->v[1].y = 0x40 + J.wob[i] / 2 + 0x10 - J.wob[i];
                s7->z = 0;
            } else {
                s7->v[0].x = i * 30 + 0x2A + J.wob[i] / 2;
                s7->v[0].y = 0x22 + J.wob[i] / 2;
                s7->v[1].x = i * 30 + 0x2A + J.wob[i] / 2 + 0x10 - J.wob[i];
                s7->v[1].y = 0x22 + J.wob[i] / 2 + 0x10 - J.wob[i];
                s7->z = 0;
            }
            s7->v[0].u = ipIconPartyRects[itemWork->eggs[i].owner].u0;
            s7->v[0].v = ipIconPartyRects[itemWork->eggs[i].owner].v0;
            s7->v[1].u = ipIconPartyRects[itemWork->eggs[i].owner].u1;
            s7->v[1].v = ipIconPartyRects[itemWork->eggs[i].owner].v1;
            s7++;
        }
    }
    s4->type = 0;
    s5->type = 0;
    s7->type = 0;
    if (FldMenuWinGetState(0xA)) {
        s4 = FldMenuWinGetSprBuf(0xA, 1);
        FldMenuWinSetLayer(0xA, 1, &fldMenuTextures[1], s4, 128.0f);
        s5 = FldMenuWinGetSprBuf(0xA, 2);
        FldMenuWinSetLayer(0xA, 2, &fldMenuTextures[4], s5, 128.0f);
        s7 = FldMenuWinGetSprBuf(0xA, 3);
        FldMenuWinSetLayer(0xA, 0, &fldMenuTextures[4], s7, 128.0f);
        s20 = FldMenuWinGetSprBuf(0xA, 4);
        FldMenuWinSetLayer(0xA, 4, &fldMenuTextures[2], s20, 128.0f);
        s24 = FldMenuWinGetSprBuf(0xA, 5);
        FldMenuWinSetLayer(0xA, 5, &fldMenuTextures[0], s24, 128.0f);
        FldMenuBarBuild((FldMenuQuad *)FldMenuWinGetQuadBuf(0xA), -0x60, 0x10, 0xC8, 0xC, itemWork->eggs[J.egg0 - 1].useClass);
        FldMenuWinTextAdd(0xA, 1, itemWork->eggs[J.egg0 - 1].name, -0x60, 0, 0x14, 0x14, cnt++, -1, 2);
        icon = (s8)itemWork->eggs[J.egg0 - 1].icon;
        for (i = 0; i < itemWork->eggs[J.egg0 - 1].rarity; i++) {
            VuSinCosDeg(&sn, &cs, i * 110.0f + J.ang);
            s5->type = 1;
            if (itemWork->eggs[J.egg0 - 1].rarity == 1) {
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
            s5->v[0].x = 0x36 + (s32)(cs * 7.0f);
            s5->z = 0;
            s5->v[0].y = 0x20 + (s32)(sn * 5.0f);
            s5->v[1].x = 0x36 + (s32)(cs * 7.0f) + 10;
            s5->v[1].y = 0x20 + (s32)(sn * 5.0f) + 10;
            s5->v[0].u = sprStarIconRects[0].u0;
            s5->v[0].v = sprStarIconRects[0].v0;
            s5->v[1].u = sprStarIconRects[0].u1;
            s5->v[1].v = sprStarIconRects[0].v1;
            s5++;
        }
        s4->type = 1;
        s4->v[0].r = J.alphaA;
        s4->v[0].g = J.alphaA;
        s4->v[0].b = J.alphaA;
        s4->v[0].a = 0;
        s4->v[0].x = 0x36;
        s4->v[0].y = J.slideA + 0x20;
        s4->v[1].x = menuItemIcons[icon].w + 0x36;
        s4->v[1].y = menuItemIcons[icon].h + 0x20;
        s4->z = 0;
        s4->v[0].u = menuItemIcons[icon].u0;
        s4->v[0].v = menuItemIcons[icon].v0;
        s4->v[1].u = menuItemIcons[icon].u1;
        s4->v[1].v = menuItemIcons[icon].v1;
        s4++;
        s4->type = 0;
        for (i = 0; i < SPELLS[J.egg0 - 1].nSpells; i++) {
            FldMenuWinTextAdd(0xA, 1, SPELLS[J.egg0 - 1].spells[i].name, -0x60, i * 16 + 0x54, 0x10, 0x10, cnt++, -1, 3);
            icon = SPELLS[J.egg0 - 1].spells[i].icon;
            s5->type = 1;
            s5->v[0].x = -0x80;
            s5->v[0].r = 0x80;
            s5->v[1].x = menuElementIcons[icon].w - 0x88;
            s5->v[1].y = i * 16 + (menuElementIcons[icon].h + 0x4C);
            s5->v[0].u = menuElementIcons[icon].u0;
            s5->v[0].v = menuElementIcons[icon].v0;
            s5->v[1].u = menuElementIcons[icon].u1;
            s5->v[1].v = menuElementIcons[icon].v1;
            s5->v[0].g = 0x80;
            s5->v[0].b = 0x80;
            s5->v[0].y = i * 16 + 0x54;
            s5->v[0].a = 0;
            s5->z = 0;
            s5++;
            if (J.tick >= 0x3C) {
                n5 = 5;
                for (k = 0; k < 5; k++) {
                    if (SPELLS[J.egg0 - 1].spells[i].ids[k]) {
                        n5--;
                    }
                }
                for (k = 0; k < 5; k++) {
                    if (SPELLS[J.egg0 - 1].spells[i].ids[k]) {
                        icon = SPELLS[J.egg0 - 1].spells[i].ids[k] - 1;
                        s7->v[0].x = n5 * 20 - 0x2C;
                        s7->type = 1;
                        s7->v[0].r = 0x80;
                        s7->v[1].x = n5 * 20 - 0x2C + fldMenuJunctionIcons[icon].w;
                        s7->v[0].g = 0x80;
                        s7->v[0].b = 0x80;
                        s7->v[1].y = i * 16 + (fldMenuJunctionIcons[icon].h + 0x54);
                        s7->v[0].u = fldMenuJunctionIcons[icon].u0;
                        s7->v[0].v = fldMenuJunctionIcons[icon].v0;
                        s7->v[1].u = fldMenuJunctionIcons[icon].u1;
                        s7->v[1].v = fldMenuJunctionIcons[icon].v1;
                        s7->v[0].a = 0;
                        s7->v[0].y = i * 16 + 0x54;
                        s7->z = 0;
                        s7++;
                        n5++;
                    }
                }
            }
        }
        s20->type = 1;
        s20->v[0].r = 0x80;
        s20->v[0].g = 0x80;
        s20->v[0].b = 0x80;
        s20->v[0].a = 0x80;
        s20->z = 0;
        s20->v[0].x = 0x54;
        s20->v[0].y = 0x20;
        s20->v[1].x = 0x72;
        s20->v[1].y = 0x34;
        s20->v[0].u = 0xCE;
        s20->v[0].v = 0x6E;
        s20->v[1].u = 0xEC;
        s20->v[1].v = 0x82;
        s20++;
        q = FldMenuNumberBuild(s24, itemWork->eggs[J.egg0 - 1].maxMp, 0, 0x78, 0x20, 1.0f, 1.0f, 0);
        s4->type = 0;
        s5->type = 0;
        s7->type = 0;
        s20->type = 0;
        q->type = 0;
    }
    if (FldMenuWinGetState(0xB)) {
        s4 = FldMenuWinGetSprBuf(0xB, 1);
        FldMenuWinSetLayer(0xB, 1, &fldMenuTextures[1], s4, 128.0f);
        s5 = FldMenuWinGetSprBuf(0xB, 2);
        FldMenuWinSetLayer(0xB, 2, &fldMenuTextures[4], s5, 128.0f);
        s7 = FldMenuWinGetSprBuf(0xB, 3);
        FldMenuWinSetLayer(0xB, 0, &fldMenuTextures[4], s7, 128.0f);
        s20 = FldMenuWinGetSprBuf(0xB, 4);
        FldMenuWinSetLayer(0xB, 4, &fldMenuTextures[2], s20, 128.0f);
        s24 = FldMenuWinGetSprBuf(0xB, 5);
        FldMenuWinSetLayer(0xB, 5, &fldMenuTextures[0], s24, 128.0f);
        FldMenuBarBuild((FldMenuQuad *)FldMenuWinGetQuadBuf(0xB), 0x20, 0x10, 0xC8, 0xC, itemWork->eggs[J.egg1 - 1].useClass);
        FldMenuWinTextAdd(0xB, 2, itemWork->eggs[J.egg1 - 1].name, 0x20, 0, 0x14, 0x14, cnt++, -1, 2);
        icon = (s8)itemWork->eggs[J.egg1 - 1].icon;
        s4->type = 1;
        s4->v[0].r = J.alphaB;
        s4->v[0].g = J.alphaB;
        s4->v[0].b = J.alphaB;
        s4->v[0].a = 0;
        s4->v[0].x = 0x36;
        s4->v[0].y = J.slideB + 0x20;
        s4->z = 0;
        s4->v[1].x = menuItemIcons[icon].w + 0x36;
        s4->v[1].y = menuItemIcons[icon].h + 0x20;
        s4->v[0].u = menuItemIcons[icon].u0;
        s4->v[0].v = menuItemIcons[icon].v0;
        s4->v[1].u = menuItemIcons[icon].u1;
        s4->v[1].v = menuItemIcons[icon].v1;
        s4++;
        s4->type = 0;
        for (i = 0; i < itemWork->eggs[J.egg1 - 1].rarity; i++) {
            VuSinCosDeg(&sn, &cs, i * 110.0f + J.ang);
            s5->type = 1;
            if (itemWork->eggs[J.egg1 - 1].rarity == 1) {
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
            s5->v[0].x = 0x36 + (s32)(cs * 7.0f);
            s5->z = 0;
            s5->v[0].y = 0x20 + (s32)(sn * 5.0f);
            s5->v[1].x = 0x36 + (s32)(cs * 7.0f) + 10;
            s5->v[1].y = 0x20 + (s32)(sn * 5.0f) + 10;
            s5->v[0].u = sprStarIconRects[0].u0;
            s5->v[0].v = sprStarIconRects[0].v0;
            s5->v[1].u = sprStarIconRects[0].u1;
            s5->v[1].v = sprStarIconRects[0].v1;
            s5++;
        }
        for (i = 0; i < SPELLS[J.egg1 - 1].nSpells; i++) {
            FldMenuWinTextAdd(0xB, 2, SPELLS[J.egg1 - 1].spells[i].name, 0x58, i * 16 + 0x54, 0x10, 0x10, cnt++, -1, 3);
            icon = SPELLS[J.egg1 - 1].spells[i].icon;
            s5->type = 1;
            s5->v[0].x = 0x40;
            s5->v[0].r = 0x80;
            s5->v[1].x = menuElementIcons[icon].w + 0x38;
            s5->v[1].y = i * 16 + (menuElementIcons[icon].h + 0x4C);
            s5->v[0].u = menuElementIcons[icon].u0;
            s5->v[0].v = menuElementIcons[icon].v0;
            s5->v[1].u = menuElementIcons[icon].u1;
            s5->v[1].v = menuElementIcons[icon].v1;
            s5->v[0].g = 0x80;
            s5->v[0].b = 0x80;
            s5->v[0].y = i * 16 + 0x54;
            s5->v[0].a = 0;
            s5->z = 0;
            s5++;
            if (J.tick >= 0x3C) {
                n5 = 5;
                for (k = 0; k < 5; k++) {
                    if (SPELLS[J.egg1 - 1].spells[i].ids[k]) {
                        n5--;
                    }
                }
                for (k = 0; k < 5; k++) {
                    if (SPELLS[J.egg1 - 1].spells[i].ids[k]) {
                        icon = SPELLS[J.egg1 - 1].spells[i].ids[k] - 1;
                        s7->v[0].x = n5 * 20 + 0xA0;
                        s7->type = 1;
                        s7->v[0].r = 0x80;
                        s7->v[1].x = n5 * 20 + 0xA0 + fldMenuJunctionIcons[icon].w;
                        s7->v[0].g = 0x80;
                        s7->v[0].b = 0x80;
                        s7->v[1].y = i * 16 + (fldMenuJunctionIcons[icon].h + 0x54);
                        s7->v[0].a = 0;
                        s7->v[0].u = fldMenuJunctionIcons[icon].u0;
                        s7->v[0].v = fldMenuJunctionIcons[icon].v0;
                        s7->v[1].u = fldMenuJunctionIcons[icon].u1;
                        s7->v[1].v = fldMenuJunctionIcons[icon].v1;
                        s7->v[0].y = i * 16 + 0x54;
                        s7->z = 0;
                        s7++;
                        n5++;
                    }
                }
            }
        }
        s20->type = 1;
        s20->v[0].r = 0x80;
        s20->v[0].g = 0x80;
        s20->v[0].b = 0x80;
        s20->v[0].a = 0x80;
        s20->z = 0;
        s20->v[0].x = 0x54;
        s20->v[0].y = 0x20;
        s20->v[1].x = 0x72;
        s20->v[1].y = 0x34;
        s20->v[0].u = 0xCE;
        s20->v[0].v = 0x6E;
        s20->v[1].u = 0xEC;
        s20->v[1].v = 0x82;
        s20++;
        q = FldMenuNumberBuild(s24, itemWork->eggs[J.egg1 - 1].maxMp, 0, 0x78, 0x20, 1.0f, 1.0f, 0);
        s4->type = 0;
        s5->type = 0;
        s7->type = 0;
        s20->type = 0;
        q->type = 0;
    }
    if (FldMenuWinGetState(0xC) && (s8)itemWork->unk6ECC == 0) {
        s4 = FldMenuWinGetSprBuf(0xC, 1);
        FldMenuWinSetLayer(0xC, 1, &fldMenuTextures[1], s4, 128.0f);
        FldMenuBarBuild((FldMenuQuad *)FldMenuWinGetQuadBuf(0xC), 0, 0x10, 0xA0, 0xC, 0);
        FldMenuWinTextAdd(0xC, 2, PREVIEW->name, 0, 0, 0x14, 0x14, cnt++, -1, 2);
        icon = PREVIEW->icon;
        s4->type = 1;
        s4->v[0].r = 0x80;
        s4->v[0].g = 0x80;
        s4->v[0].b = 0x80;
        s4->v[0].a = 0;
        s4->z = 0;
        s4->v[0].x = 0x36;
        s4->v[0].y = 0x20;
        s4->v[1].x = menuItemIcons[icon].w + 0x36;
        s4->v[1].y = menuItemIcons[icon].h + 0x20;
        s4->v[0].u = menuItemIcons[icon].u0;
        s4->v[0].v = menuItemIcons[icon].v0;
        s4->v[1].u = menuItemIcons[icon].u1;
        s4->v[1].v = menuItemIcons[icon].v1;
        s4++;
        s4->type = 0;
        s5 = FldMenuWinGetSprBuf(0xC, 4);
        FldMenuWinSetLayer(0xC, 4, &fldMenuTextures[4], s5, 128.0f);
        s7 = FldMenuWinGetSprBuf(0xC, 0);
        FldMenuWinSetLayer(0xC, 0, &fldMenuTextures[4], s7, 128.0f);
        for (i = 0; i < PREVIEW->nSpells; i++) {
            FldMenuWinTextAdd(0xC, 2, PREVIEW->spells[i].name, 0x10, i * 16 + 0x40, 0x10, 0x10, cnt++, -1, 1);
            icon = PREVIEW->spells[i].icon;
            s5->v[0].r = 0x80;
            s5->v[0].g = 0x80;
            s5->v[0].b = 0x80;
            s5->v[0].a = 0;
            s5->type = 1;
            s5->v[0].x = -0x10;
            s5->v[0].y = i * 16 + 0x40;
            s5->v[1].x = menuElementIcons[icon].w - 0x18;
            s5->v[1].y = i * 16 + (menuElementIcons[icon].h + 0x38);
            s5->v[0].u = menuElementIcons[icon].u0;
            s5->v[0].v = menuElementIcons[icon].v0;
            s5->v[1].u = menuElementIcons[icon].u1;
            s5->v[1].v = menuElementIcons[icon].v1;
            s5->z = 0;
            s5++;
        }
        s4->type = 0;
        s5->type = 0;
        s7->type = 0;
        s4 = FldMenuWinGetSprBuf(0xC, 3);
        FldMenuWinSetLayer(0xC, 3, &fldMenuTextures[2], s4, 128.0f);
        s5 = FldMenuWinGetSprBuf(0xC, 2);
        FldMenuWinSetLayer(0xC, 2, &fldMenuTextures[0], s5, 128.0f);
        s4->type = 1;
        s4->v[0].r = 0x80;
        s4->v[0].g = 0x80;
        s4->v[0].b = 0x80;
        s4->v[0].a = 0x80;
        s4->z = 0;
        s4->v[0].x = 0x54;
        s4->v[0].y = 0x20;
        s4->v[1].x = 0x72;
        s4->v[1].y = 0x34;
        s4->v[0].u = 0xCE;
        s4->v[0].v = 0x6E;
        s4->v[1].u = 0xEC;
        s4->v[1].v = 0x82;
        s4++;
        q = FldMenuNumberBuild(s5, PREVIEW->mp, 0, 0x78, 0x20, 1.0f, 1.0f, 0);
        s4->type = 0;
        q->type = 0;
    }
    if (count >= 2) {
        switch (J.state) {
        case 0:
            FldMenuCursorSetAt(9, (f32)(J.cursor * 30) + 24.0f, (f32)(J.page * 34) + 32.0f);
            break;
        case 1:
            FldMenuCursorSetAt(0xD, 400.0f - (f32)(J.confirm * 10), (f32)(J.confirm * 20) + 64.0f);
            break;
        default:
            FldMenuCursorReset();
            break;
        }
    } else {
        FldMenuCursorReset();
    }
    s4 = FldMenuWinGetSprBuf(8, 0);
    FldMenuWinSetLayer(8, 0xF, FldOverlayImgGetTex(0), s4, 128.0f);
    switch (J.state) {
    case 0:
    case 1:
    case 4:
        if (J.fade >= 0x40) {
            J.fade--;
        }
        break;
    case 2:
    case 3:
        if (J.fade < 0x80) {
            J.fade++;
        }
        break;
    }
    for (i = 0; i < 32; i++) {
        s4->type = 1;
        if (J.state == 3) {
            if (i == 31) {
                if (J.timer >= 400) {
                    s4->v[0].r = 0x7C;
                    s4->v[0].g = 0x7C;
                    s4->v[0].b = 0x7C;
                    s4->v[0].a = 0;
                    s4->v[1].r = 0x7C;
                    s4->v[1].g = 0x7C;
                    s4->v[1].b = 0x7C;
                    s4->v[1].a = 0;
                    s4->v[2].r = 0x7C;
                    s4->v[2].g = 0x7C;
                    s4->v[2].b = 0x7C;
                    s4->v[2].a = 0;
                    s4->v[3].r = 0x7C;
                    s4->v[3].g = 0x7C;
                    s4->v[3].b = 0x7C;
                    s4->v[3].a = 0;
                } else if (J.timer < 200) {
                    s4->v[0].r = J.timer / 4 + 0x7C;
                    s4->v[0].g = 0x7C;
                    s4->v[0].b = 0x7C;
                    s4->v[0].a = 0;
                    s4->v[1].r = J.timer / 4 + 0x7C;
                    s4->v[1].g = 0x7C;
                    s4->v[1].b = 0x7C;
                    s4->v[1].a = 0;
                    s4->v[2].r = J.timer / 4 + 0x7C;
                    s4->v[2].g = 0x7C;
                    s4->v[2].b = 0x7C;
                    s4->v[2].a = 0;
                    s4->v[3].r = J.timer / 4 + 0x7C;
                    s4->v[3].g = 0x7C;
                    s4->v[3].b = 0x7C;
                    s4->v[3].a = 0;
                } else {
                    s4->v[0].r = -0x20 - (s32)(J.timer / 4);
                    s4->v[0].g = 0x7C;
                    s4->v[0].b = 0x7C;
                    s4->v[0].a = 0;
                    s4->v[1].r = -0x20 - (s32)(J.timer / 4);
                    s4->v[1].g = 0x7C;
                    s4->v[1].b = 0x7C;
                    s4->v[1].a = 0;
                    s4->v[2].r = -0x20 - (s32)(J.timer / 4);
                    s4->v[2].g = 0x7C;
                    s4->v[2].b = 0x7C;
                    s4->v[2].a = 0;
                    s4->v[3].r = -0x20 - (s32)(J.timer / 4);
                    s4->v[3].g = 0x7C;
                    s4->v[3].b = 0x7C;
                    s4->v[3].a = 0;
                }
            } else {
                s4->v[0].r = i * 4;
                s4->v[0].g = i * 4;
                s4->v[0].b = i * 4;
                s4->v[0].a = 0;
                s4->v[1].r = i * 4;
                s4->v[1].g = i * 4;
                s4->v[1].b = i * 4;
                s4->v[1].a = 0;
                s4->v[2].r = i * 4;
                s4->v[2].g = i * 4;
                s4->v[2].b = i * 4;
                s4->v[2].a = 0;
                s4->v[3].r = i * 4;
                s4->v[3].g = i * 4;
                s4->v[3].b = i * 4;
                s4->v[3].a = 0;
            }
        } else {
            s4->v[0].r = J.fade;
            s4->v[0].g = J.fade;
            s4->v[0].b = J.fade;
            s4->v[0].a = 0;
            s4->v[1].r = J.fade;
            s4->v[1].g = J.fade;
            s4->v[1].b = J.fade;
            s4->v[1].a = 0;
            s4->v[2].r = J.fade;
            s4->v[2].g = J.fade;
            s4->v[2].b = J.fade;
            s4->v[2].a = 0;
            s4->v[3].r = J.fade;
            s4->v[3].g = J.fade;
            s4->v[3].b = J.fade;
            s4->v[3].a = 0;
        }
        s4->v[0].x = 0;
        s4->v[0].y = 0;
        s4->v[1].x = 0;
        s4->v[1].y = 0x100;
        s4->v[2].x = 0x100;
        s4->v[2].y = 0;
        s4->v[3].x = 0x100;
        s4->v[3].y = 0x100;
        s4->z = 0;
        if (J.state == 3) {
            FldMenuJunctionMeshStep(0.003f);
            amp = fldMenuJunctionWarpAmp;
            s4->v[0].x += (s32)(fldMenuJunctionMeshX[1][1] * (f32)(i * amp));
            s4->v[0].y += (s32)(fldMenuJunctionMeshX[2][2] * (f32)(i * (amp * 2)));
            s4->v[2].x += (s32)(fldMenuJunctionMeshX[3][1] * (f32)(i * amp));
            s4->v[2].y += (s32)(fldMenuJunctionMeshX[1][3] * (f32)(i * (amp * 2)));
        }
        s4->v[0].u = 0;
        s4->v[0].v = 0;
        s4->v[1].u = 0;
        s4->v[1].v = 0x100;
        s4->v[2].u = 0x100;
        s4->v[2].v = 0;
        s4->v[3].u = 0x100;
        s4->v[3].v = 0x100;
        s4++;
    }
    s4->type = 0;
    return 0x10;
}
