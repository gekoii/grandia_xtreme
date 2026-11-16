/* Port-only C for FldMenuItemUpdate (0x00628EC0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "sys.h"
#include "field/menu.h"
#include "field/event.h"

/*
 * Based on the decompilation rounds' near miss (2208 of 2208 instructions,
 * the differences all scheduling in case 1's icon sprite). field/n23.h's
 * FldMenuItem is not included: this screen reads more of the block, and
 * reads +0xC8 as a byte and +0xD4 as a word, so it has its own view here.
 */
typedef struct FldMenuItemView {
    /* 0x00 */ s8 category;        /* list shown: 0 bag, 1 eggs, 2 books, 3 skills, 4 valuables, 5 */
    /* 0x01 */ s8 state;           /* 0 list, 1 using an item on a member, 2 command window, 3 sub-list */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s32 row;            /* party panel cursor (FldMenuPartySelect) */
    /* 0x08 */ s32 col;
    /* 0x0C */ u32 top;            /* first list row shown */
    /* 0x10 */ u32 cursor;
    /* 0x14 */ u32 sub;            /* command window / sub-list cursor */
    /* 0x18 */ u8 skillGauge;      /* counts up every frame (FldMenuItemListLayout) */
    /* 0x19 */ u8 pipCur;          /* pip growing now */
    /* 0x1A */ u8 pipLen[10];
    /* 0x24 */ u8 pipTimer[10];    /* 0..199 */
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 blink;           /* tab bob, every other frame */
    /* 0x31 */ u8 bobDir;
    /* 0x32 */ s8 bob[6];          /* per category tab */
    /* 0x38 */ s8 marks[0x80];     /* FldMenuItemListLayout */
    /* 0xB8 */ s8 rowX[8];         /* sub-list row slide-in offsets */
    /* 0xC0 */ u8 timer;           /* frames left of the item use flash */
    /* 0xC1 */ u8 help;            /* help as a popup at the cursor */
    /* 0xC2 */ u8 padC2[2];
    /* 0xC4 */ s32 last;           /* -1 forces a help redraw */
    /* 0xC8 */ s8 helpType;        /* s32 in field/n23.h; this screen reads the low byte */
    /* 0xC9 */ u8 padC9[3];
    /* 0xCC */ s32 helpId;
    /* 0xD0 */ u16 ang;
    /* 0xD2 */ u8 padD2[2];
    /* 0xD4 */ s32 page;           /* window pair in use (8/9); u8 in field/n23.h */
    /* 0xD8 */ s32 useIcon;        /* bag item being used: ItemBagRec +2, +3, +6, +0xC */
    /* 0xDC */ s32 useCount;
    /* 0xE0 */ s32 useLocked;
    /* 0xE4 */ s32 useName;
} FldMenuItemView; /* size 0xE8 */

extern FldMenuItemView fldMenuItem;

/* itemWork's lists as this screen reads them (field/menu.h's records with
 * the signedness of the loads here). */
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s8 unk2;            /* icon */
    /* 0x03 */ s8 count;
    /* 0x04 */ u8 pad4;
    /* 0x05 */ s8 unk5;            /* use target (5, 6 the whole party) */
    /* 0x06 */ s8 unk6;            /* not usable here */
    /* 0x07 */ u8 pad7[3];
    /* 0x0A */ s16 unkA;           /* bag slot */
    /* 0x0C */ s32 unkC;           /* name */
    /* 0x10 */ u8 pad10[8];
} ItemBagView; /* ItemBagRec */

typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s8 icon;
    /* 0x03 */ u8 pad3[2];
    /* 0x05 */ u8 useClass;
    /* 0x06 */ s16 index;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s32 name;
    /* 0x14 */ u8 pad14[4];
} ItemEggView; /* ItemEggListRec */

typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s8 icon;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u8 type;
    /* 0x05 */ u8 pad5[5];
    /* 0x0A */ u16 id;             /* ItemSkillRec.index */
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ u16 unkE;           /* ItemBookRec.index */
    /* 0x10 */ s32 name;
    /* 0x14 */ u8 pad14[8];
} ItemRecView; /* ItemRec (books, books2) */

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 count;
    /* 0x02 */ u8 on[16];          /* ItemRow a[8], b[8] read as one array */
} ItemRowView; /* ItemRow */

typedef struct {
    /* 0x0000 */ u8 pad0[0x1140];
    /* 0x1140 */ ItemBagView bag[30];
    /* 0x1410 */ ItemEggView eggs[24];
    /* 0x1650 */ ItemRecView books[24];
    /* 0x18F0 */ ItemRecView books2[24];
    /* 0x1B90 */ u8 pad1B90[0x2500 - 0x1B90];
    /* 0x2500 */ ItemRowView rows[5];
} ItemListWork;

#define LIST ((ItemListWork *)itemWork)

/* Sub-list sizes and message ids per row (FldMenuItemCmdTextsInit). */
typedef struct {
    u8 n;
    u8 pad;
    u16 id[8];
} FldMenuItemCmdText;

extern FldMenuItemCmdText fldMenuItemCmdTexts[5];
extern u8 fldMenuItemGuideModes[6];
extern const s8 fldMenuItemTabTitleX[6];
extern const u16 fldMenuItemTabIcons[6];
extern GsTexPacket fldMenuTextures[9];

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);
FldMenuQuad *FldMenuTaggedNumberBuild(FldMenuQuad *, s32, s32, s32, s32, s32, s32, f32, f32);
void FldMenuPartyPanelOpen(s32, s32, s32);

s32 FldMenuItemUpdate(void) {
    s32 cnt;
    s32 sel;
    f32 fx;
    f32 fy;
    EvtScnrSndStatus *snd;
    s32 n;
    s32 other;
    s32 i;
    s32 k;
    s32 nn;
    s32 lim;
    s32 txt;
    u8 col;
    s8 icon;
    s32 ok;
    s32 idx;
    s32 id;
    s8 count;
    s8 target;
    u16 trig;
    s32 w;
    s32 slot;
    FldMenuQuad *p;
    FldMenuQuad *q;
    FldMenuQuad *r1;
    FldMenuQuad *r2;

    n = 0;
    snd = evtScnrSndStatus;
    fldMenuItem.skillGauge++;
    sel = 0;
    cnt = 0;
    fldMenuItem.ang += 5;
    other = fldMenuItem.page ^ 1;
    if (fldMenuItem.ang >= 360) {
        fldMenuItem.ang -= 360;
    }

    for (i = 0; i < 10; i++) {
        fldMenuItem.pipTimer[i]++;
        if (fldMenuItem.pipTimer[i] >= 200) {
            fldMenuItem.pipTimer[i] = 0;
        }
    }

    for (i = 0; i < 10; i++) {
        if (fldMenuItem.pipCur == i) {
            if (fldMenuItem.pipLen[i] < sprStarIconRects[0].w) {
                fldMenuItem.pipLen[i] += 2;
            }
            if (fldMenuItem.pipLen[i] >= sprStarIconRects[0].w) {
                if (++fldMenuItem.pipCur == 9) {
                    fldMenuItem.pipCur = 0;
                }
            }
        } else if (fldMenuItem.pipLen[i] > sprStarIconRects[0].w / 2) {
            fldMenuItem.pipLen[i]--;
        }
    }

    switch (fldMenuItem.state) {
    case 0:
        if (padPort0[0].trigger & 4) {
            FldMenuWinClose(fldMenuItem.page + 8, 0, 0);
            fldMenuItem.category--;
            FldMenuItemReset();
            if (fldMenuItem.category < 0) {
                fldMenuItem.category = 5;
            }
            fldMenuItem.page ^= 1;
            FldMenuItemListWinOpen(fldMenuItem.page + 8, 1);
            MenuLrArrowsFlash(1);
            FldMenuGuideSet(fldMenuItemGuideModes[fldMenuItem.category]);
            SndSePlay(0x7D5, 0, 0, 0x1E, 0);
        } else if (padPort0[0].trigger & 8) {
            FldMenuWinClose(fldMenuItem.page + 8, 0, 1);
            fldMenuItem.category++;
            FldMenuItemReset();
            if (fldMenuItem.category >= 6) {
                fldMenuItem.category = 0;
            }
            fldMenuItem.page ^= 1;
            FldMenuItemListWinOpen(fldMenuItem.page + 8, 0);
            MenuLrArrowsFlash(0);
            FldMenuGuideSet(fldMenuItemGuideModes[fldMenuItem.category]);
            SndSePlay(0x7D5, 0, 0, 0x1E, 0);
        } else {
            FldMenuItemGetCount((u32 *)&cnt, &sel);
            if (padPort0[0].trigger & 0x40) {
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                return 1;
            }
            if (cnt != 0) {
                if (padPort0[0].trigger & 0x10) {
                    if (fldMenuItem.help != 0) {
                        fldMenuItem.help = 0;
                    } else {
                        fldMenuItem.help = 1;
                    }
                    fldMenuItem.last = -1;
                }
                if (padPort0[0].trigger & 0x80) {
                    if (fldMenuItem.category == 0 || fldMenuItem.category == 2 || fldMenuItem.category == 1 ||
                        fldMenuItem.category == 3) {
                        FldMenuWinOpen(0xB, 0x140, 0xE0, 0x1A4, 0xA4, 0, 8, 0x8011);
                        fldMenuItem.state = 2;
                        FldMenuWinSetActive(0xB);
                        fldMenuItem.sub = 1;
                        SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                    } else {
                        SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                    }
                } else if (padPort0[0].trigger & 0x20) {
                    switch (fldMenuItem.category) {
                    case 0:
                        if (LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unk6 != 0) {
                            SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                        } else {
                            fldMenuItem.state = 1;
                            FldMenuPartyPanelOpen(0xD, 0x140, 0x12C);
                            FldMenuWinOpen(0xC, 0x140, 0x118, 0x244, 0x12C, 0, 8, 0x11);
                            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                            fldMenuItem.useIcon = LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unk2;
                            fldMenuItem.useName = LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unkC;
                            fldMenuItem.useCount = LIST->bag[fldMenuItem.top + fldMenuItem.cursor].count;
                            fldMenuItem.useLocked = LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unk6;
                        }
                        break;
                    case 5:
                        nn = 0;
                        for (i = 0; i < LIST->rows[fldMenuItem.cursor].count; i++) {
                            if (LIST->rows[fldMenuItem.cursor].on[i] != 0) {
                                nn++;
                            }
                        }
                        if (nn != 0) {
                            fldMenuItem.state = 3;
                            FldMenuWinOpen(0xB, 0x140, 0xE0, 0xE0, fldMenuItemCmdTexts[fldMenuItem.cursor].n * 0x18 + 0x10,
                                           0, 8, 0x88011);
                            FldMenuWinSetActive(0xB);
                            fldMenuItem.sub = 0;
                            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                        } else {
                            SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                        }
                        break;
                    default:
                        SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                        break;
                    }
                }
            }
        }
        break;
    case 1:
        FldMenuPartyPanelUpdate(0xD);
        FldMenuWinTextClear(0xC);
        idx = fldMenuItem.top + fldMenuItem.cursor;
        p = (FldMenuQuad *)FldMenuWinGetQuadBuf(0xC);
        FldMenuBarBuild(p, 0x10, 0x18, 0x1AE, 0xC, fldMenuItem.useLocked);
        p = FldMenuWinGetSprBuf(0xC, 2);
        FldMenuWinSetLayer(0xC, 2, &fldMenuTextures[0], p, 128.0f);
        if (LIST->bag[idx].unk6 != 1) {
            FldMenuWinTextAdd(0xC, other, sysGrxsysPac->msg->tbl[0], 0x12C, 8, 0x12, 0x14, 1, 0x19, 1);
            FldMenuTaggedNumberBuild(p, fldMenuItem.useCount, 0, 0x180, 4, 0, 0, 1.0f, 1.0f);
        }
        q = FldMenuWinGetSprBuf(0xC, 0);
        FldMenuWinSetLayer(0xC, 0, &fldMenuTextures[1], q, 128.0f);
        id = fldMenuItem.useIcon;
        q->type = 1;
        q->v[0].r = 0x80;
        q->v[0].g = 0x80;
        q->v[0].b = 0x80;
        q->v[0].a = 0;
        q->v[0].x = 0x20;
        q->v[0].y = 4;
        q->v[1].x = menuItemIcons[id].w + 0x20;
        q->v[1].y = menuItemIcons[id].h + 4;
        q->v[0].u = menuItemIcons[id].u0;
        q->v[0].v = menuItemIcons[id].v0;
        q->v[1].u = menuItemIcons[id].u1;
        q->v[1].v = menuItemIcons[id].v1;
        q->z = 0;
        q[1].type = 0;
        FldMenuWinTextAdd(0xC, other, fldMenuItem.useName, 0x40, 6, 0x16, 0x16, 0, -1, 8);
        FldMenuWinTextAdd(0xC, other, sysGrxsysPac->msg->tbl[0], 0x60, 0x22, 0x18, 0x18, 2, 0x18, 3);
        FldMenuPartySelect(0xD, &fldMenuItem.row, &fldMenuItem.col);
        if (fldMenuItem.timer == 0) {
            if (padPort0[0].trigger & 0x40) {
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                fldMenuItem.state = 0;
            }
            if (padPort0[0].trigger & 0x20) {
                if (ItemCanUseInField(LIST->bag[idx].unkA) >= 0) {
                    count = LIST->bag[idx].count;
                    target = LIST->bag[idx].unk5;
                    if (ItemUseInField(LIST->bag[idx].unkA, fldMenuItem.col + fldMenuItem.row * 2) >= 0) {
                        if (target == 5 || target == 6) {
                            FldMenuPartyPanelFlash(0);
                            FldMenuPartyPanelFlash(1);
                            FldMenuPartyPanelFlash(2);
                            FldMenuPartyPanelFlash(3);
                        } else {
                            FldMenuPartyPanelFlash(fldMenuItem.col + fldMenuItem.row * 2);
                        }
                        SndSePlay(0x7D8, 0, 0, 0x1E, 0);
                        if (count == 1) {
                            /* the last one: the slot is gone */
                            fldMenuItem.timer = 0x3C;
                        } else {
                            fldMenuItem.useCount = LIST->bag[idx].count;
                        }
                    } else {
                        SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                    }
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            }
        } else {
            fldMenuItem.timer--;
            trig = padPort0[0].trigger;
            if (trig & 0x20) {
                fldMenuItem.timer = 0;
            }
            if (trig & 0x40) {
                fldMenuItem.timer = 0;
            }
            if (fldMenuItem.timer == 0) {
                fldMenuItem.state = 0;
            }
        }
        if (fldMenuItem.state == 0) {
            for (i = 0; i < 5; i++) {
                FldMenuWinClose(i + 0xC, i * 2, 3);
            }
            FldMenuWinSetActive(fldMenuItem.page + 8);
        }
        break;
    case 2:
        switch (fldMenuItem.category) {
        case 0:
        default:
            icon = LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unk2;
            txt = LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unkC;
            col = LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unk6;
            break;
        case 2:
            txt = LIST->books[fldMenuItem.top + fldMenuItem.cursor].name;
            col = LIST->books[fldMenuItem.top + fldMenuItem.cursor].type + 0xB;
            icon = LIST->books[fldMenuItem.top + fldMenuItem.cursor].icon;
            break;
        case 1:
            icon = LIST->eggs[fldMenuItem.top + fldMenuItem.cursor].icon;
            txt = LIST->eggs[fldMenuItem.top + fldMenuItem.cursor].name;
            col = LIST->eggs[fldMenuItem.top + fldMenuItem.cursor].useClass;
            break;
        case 3:
            txt = LIST->books2[fldMenuItem.top + fldMenuItem.cursor].name;
            col = LIST->books2[fldMenuItem.top + fldMenuItem.cursor].type + 0xB;
            icon = LIST->books2[fldMenuItem.top + fldMenuItem.cursor].icon;
            break;
        }
        FldMenuCursorSetAt(0xB, 128.0f, (f32)(fldMenuItem.sub * 0x18) + 104.0f);
        r1 = FldMenuWinGetSprBuf(0xB, 3);
        FldMenuWinSetLayer(0xB, 3, &fldMenuTextures[4], r1, 128.0f);
        q = FldMenuWinGetSprBuf(0xB, 1);
        FldMenuWinSetLayer(0xB, 1, &fldMenuTextures[1], q, 128.0f);
        r2 = FldMenuWinGetSprBuf(0xB, 2);
        FldMenuWinSetLayer(0xB, 2, &fldMenuTextures[0], r2, 128.0f);
        if (fldMenuItem.category == 0) {
            if (LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unk6 != 1) {
                r2 = FldMenuTaggedNumberBuild(r2, LIST->bag[fldMenuItem.top + fldMenuItem.cursor].count, 0, 0x100, 8,
                                              0, 0, 1.0f, 1.0f);
            }
        }
        FldMenuWinTextClear(0xB);
        p = (FldMenuQuad *)FldMenuWinGetQuadBuf(0xB);
        p = FldMenuBarBuild(p, 0x10, 0x18, 0x120, 0xC, col);
        /* the last argument is the text index after the increment (1, 3) */
        FldMenuWinTextAdd(0xB, other, txt, 0x30, 8, 0x14, 0x14, n, -1, n + 1);
        n++;
        FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[5], 0x20, 0x28, 0x18, 0x18, n++, fldMenuItem.category * 2,
                          3);
        FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[5], 0x20, 0x40, 0x18, 0x18, n,
                          fldMenuItem.category * 2 + 1, n + 1);
        n++;
        if (fldMenuItem.sub == 0) {
            FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[5], 0xB4, 0x60, 0x18, 0x18, n++, 8, 8);
            FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[5], 0xB4, 0x78, 0x18, 0x18, n++, 9, 0);
        } else {
            FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[5], 0xB4, 0x60, 0x18, 0x18, n++, 8, 0);
            FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[5], 0xB4, 0x78, 0x18, 0x18, n++, 9, 8);
        }
        if (fldMenuItem.category == 3) {
            r1->type = 1;
            r1->v[0].r = 0x80;
            r1->v[0].g = 0x80;
            r1->v[0].b = 0x80;
            r1->v[0].a = 0;
            r1->v[0].x = 8;
            r1->v[0].y = 8;
            r1->v[1].x = menuSkillTypeIcons[icon].w + 8;
            r1->v[1].y = menuSkillTypeIcons[icon].h + 8;
            r1->v[0].u = menuSkillTypeIcons[icon].u0;
            r1->v[0].v = menuSkillTypeIcons[icon].v0;
            r1->v[1].u = menuSkillTypeIcons[icon].u1;
            r1->v[1].v = menuSkillTypeIcons[icon].v1;
            r1->z = 0;
            r1++;
        } else {
            q->type = 1;
            q->v[0].r = 0x80;
            q->v[0].g = 0x80;
            q->v[0].b = 0x80;
            q->v[0].a = 0;
            q->v[0].x = 8;
            q->v[0].y = 8;
            q->v[1].x = menuItemIcons[icon].w + 8;
            q->v[1].y = menuItemIcons[icon].h + 8;
            q->v[0].u = menuItemIcons[icon].u0;
            q->v[0].v = menuItemIcons[icon].v0;
            q->v[1].u = menuItemIcons[icon].u1;
            q->v[1].v = menuItemIcons[icon].v1;
            q->z = 0;
            q++;
        }
        p->type = 0;
        q->type = 0;
        r2->type = 0;
        r1->type = 0;
        if (padPort0[0].trigger & 0x40) {
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            FldMenuWinClose(0xB, 0, 3);
            FldMenuWinSetActive(fldMenuItem.page + 8);
            fldMenuItem.state = 0;
        } else if (padPort0[0].trigger & 0x20) {
            ok = 0;
            if (fldMenuItem.sub == 0) {
                /* "Discard" */
                switch (fldMenuItem.category) {
                case 0:
                    ok = 1;
                    ItemBagRemove(LIST->bag[fldMenuItem.top + fldMenuItem.cursor].unkA,
                                  LIST->bag[fldMenuItem.top + fldMenuItem.cursor].count);
                    break;
                case 2:
                    if (ItemBookCheckDiscard(LIST->books[fldMenuItem.top + fldMenuItem.cursor].unkE) == 0) {
                        ok = 1;
                        ItemBookDiscard(LIST->books[fldMenuItem.top + fldMenuItem.cursor].unkE);
                    }
                    break;
                case 1:
                    /* the asm passes the egg's index; the stub takes nothing (protos/field.h) */
                    if (ItemEggCheckDiscard() == 0) {
                        ok = 1;
                        ItemEggDiscard(LIST->eggs[fldMenuItem.top + fldMenuItem.cursor].index);
                    }
                    break;
                case 3:
                    if (ItemSkillCheckDiscard(LIST->books2[fldMenuItem.top + fldMenuItem.cursor].id) == 0) {
                        ok = 1;
                        ItemSkillDiscard(LIST->books2[fldMenuItem.top + fldMenuItem.cursor].id);
                    }
                    break;
                }
            } else {
                ok = 1;
            }
            if (ok != 0) {
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                FldMenuWinClose(0xB, 0, 3);
                FldMenuWinSetActive(fldMenuItem.page + 8);
                fldMenuItem.state = 0;
            } else {
                SndSePlay(0x7D4, 0, 0, 0x1E, 0);
            }
        } else if ((padPort0[0].trigger & 0x1000) && fldMenuItem.sub != 0) {
            fldMenuItem.sub = 0;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (padPort0[0].trigger & 0x4000) {
            if (fldMenuItem.sub == 0) {
                fldMenuItem.sub = 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        }
        break;
    case 3:
        FldMenuWinTextClear(0xB);
        p = (FldMenuQuad *)FldMenuWinGetQuadBuf(0xB);
        cnt = fldMenuItemCmdTexts[fldMenuItem.cursor].n;
        q = FldMenuWinGetSprBuf(0xB, 1);
        FldMenuWinSetLayer(0xB, 1, &fldMenuTextures[1], q, 128.0f);
        FldMenuCursorSetAt(0xB, (f32)((cnt - fldMenuItem.sub) * 6) + 16.0f, (f32)(fldMenuItem.sub * 0x18) + 16.0f);
        for (i = 0; i < LIST->rows[fldMenuItem.cursor].count; i++) {
            if (fldMenuItem.sub == i) {
                lim = (cnt - i) * 6;
                fldMenuItem.rowX[i] -= 8;
                if (fldMenuItem.rowX[i] < lim) {
                    fldMenuItem.rowX[i] = lim;
                }
            } else {
                lim = (cnt - i) * 6 + 0x10;
                fldMenuItem.rowX[i] += 8;
                if (fldMenuItem.rowX[i] >= lim) {
                    fldMenuItem.rowX[i] = lim;
                }
            }
            if (LIST->rows[fldMenuItem.cursor].on[i] != 0) {
                if (fldMenuItem.sub == i) {
                    FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[14], fldMenuItem.rowX[i] + 0x30, i * 0x18 + 8,
                                      0x14, 0x14, n++, i + 6, 8);
                } else {
                    FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[14], fldMenuItem.rowX[i] + 0x30, i * 0x18 + 8,
                                      0x14, 0x14, n++, i + 6, 0);
                }
            } else {
                FldMenuWinTextAdd(0xB, other, sysGrxsysPac->msg->tbl[14], fldMenuItem.rowX[i] + 0x30, i * 0x18 + 8,
                                  0x14, 0x14, n++, i + 6, 0xA);
            }
            p = FldMenuBarBuild(p, fldMenuItem.rowX[i] + 0x10, i * 0x18 + 0x18, 0x80, 0xC, 0);
            id = fldMenuItem.cursor + 6;
            q->type = 1;
            q->v[0].r = 0x80;
            q->v[0].g = 0x80;
            q->v[0].b = 0x80;
            q->v[0].a = 0;
            q->v[0].x = fldMenuItem.rowX[i] + 0x10;
            q->v[0].y = i * 0x18 + 8;
            q->v[1].x = menuItemIcons[id].w + 0x10 + fldMenuItem.rowX[i];
            q->v[1].y = i * 0x18 + (menuItemIcons[id].h + 8);
            q->v[0].u = menuItemIcons[id].u0;
            q->v[0].v = menuItemIcons[id].v0;
            q->v[1].u = menuItemIcons[id].u1;
            q->v[1].v = menuItemIcons[id].v1;
            q->z = 0;
            q++;
        }
        q->type = 0;
        if (padPort0[0].trigger & 0x20) {
            if (LIST->rows[fldMenuItem.cursor].on[fldMenuItem.sub] != 0) {
                if (snd[1].state == 0 || snd[1].state == 2) {
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                    FldSndStreamPlay(fldMenuItemCmdTexts[fldMenuItem.cursor].id[fldMenuItem.sub]);
                }
            }
        } else if (padPort0[0].trigger & 0x40) {
            if (snd[1].state == 0) {
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                FldMenuWinClose(0xB, 0, 3);
                FldMenuWinSetActive(fldMenuItem.page + 8);
                fldMenuItem.state = 0;
            } else if (snd[1].state == 2) {
                FldSndStreamStop();
            }
        } else if (padPort0[0].trigger & 0x1000) {
            if (fldMenuItem.sub != 0) {
                fldMenuItem.sub--;
            } else {
                fldMenuItem.sub = fldMenuItemCmdTexts[fldMenuItem.cursor].n - 1;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (padPort0[0].trigger & 0x4000) {
            if (fldMenuItem.sub < fldMenuItemCmdTexts[fldMenuItem.cursor].n - 1) {
                fldMenuItem.sub++;
            } else {
                fldMenuItem.sub = 0;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
        break;
    }

    FldMenuItemGetCount((u32 *)&cnt, &sel);
    FldMenuListFixOnRemove((s32 *)&fldMenuItem.cursor, (s32 *)&fldMenuItem.top, 7, 3, cnt);
    if (cnt != 0) {
        if (fldMenuItem.state == 0) {
            if (padPort0[0].repeatFast & 0x1000) {
                FldMenuListCursorUp(&fldMenuItem.cursor, &fldMenuItem.top, 7, 3, cnt);
            }
            if (padPort0[0].repeatFast & 0x4000) {
                FldMenuListCursorDown(&fldMenuItem.cursor, &fldMenuItem.top, 7, 3, cnt);
            }
            if (cnt >= 7) {
                FldMenuCursorSetAt(fldMenuItem.page + 8, (f32)((7 - fldMenuItem.cursor) * 6) + 16.0f,
                                   (f32)(fldMenuItem.cursor * 0x18) + 20.0f);
            } else {
                FldMenuCursorSetAt(fldMenuItem.page + 8, (f32)((cnt - fldMenuItem.cursor) * 6) + 16.0f,
                                   (f32)(fldMenuItem.cursor * 0x18) + 20.0f);
            }
        }
    } else {
        FldMenuCursorSetAt(0x11, 32.0f, 12.0f);
    }

    k = 1;
    FldMenuWinTextClear(fldMenuItem.page + 8);
    FldMenuWinTextAdd(fldMenuItem.page + 8, fldMenuItem.page, sysGrxsysPac->msg->tbl[0],
                      fldMenuItemTabTitleX[fldMenuItem.category] + 0x168, 0xBC, 0x18, 0x14, 0,
                      fldMenuItem.category * 2 + 0xD, 6);
    if (cnt == 0) {
        k = 2;
        w = TxtMsgGetWidth((struct TxtMsgHdr *)sysGrxsysPac->msg->tbl[0], fldMenuItem.category + 0x1A, 0x18);
        FldMenuWinTextAdd(fldMenuItem.page + 8, fldMenuItem.page, sysGrxsysPac->msg->tbl[0], 0x100 - w / 2, 0x58,
                          0x18, 0x14, 1, fldMenuItem.category + 0x1A, 3);
    }
    k = FldMenuItemListLayout(fldMenuItem.page + 8, k, cnt);
    FldMenuCursorGetPos(&fx, &fy);
    if (fldMenuItem.state != 0) {
        FldMenuLrArrowsUnlock();
    } else {
        FldMenuWinLrArrowsSet(fldMenuItem.page + 8, 1, 12.0f);
    }
    if (fldMenuItem.state != 0) {
        fldMenuItem.helpId = 0;
    }
    if (FldMenuWinGetState(fldMenuItem.page + 8) != 4) {
        fldMenuItem.helpId = 0;
        fldMenuItem.last = -1;
    }
    if (fldMenuItem.helpId != 0) {
        if (fldMenuItem.last != fldMenuItem.cursor + fldMenuItem.top) {
            fldMenuItem.last = fldMenuItem.cursor + fldMenuItem.top;
            FldMenuCursorGetPos(&fx, &fy);
            if (fldMenuItem.help != 0) {
                MenuHelpOpen(0, fldMenuItem.helpType, (void *)fldMenuItem.helpId, fx, fy + 16.0f);
            } else {
                MenuHelpOpen(1, fldMenuItem.helpType, (void *)fldMenuItem.helpId, 32.0f, 348.0f);
            }
        }
    } else {
        fldMenuItem.last = -1;
        MenuHelpClose();
    }

    /* info window: category title, list count */
    FldMenuWinTextClear(0xA);
    if (fldMenuItem.category == 0) {
        slot = 0x19;
        w = 0x1B;
    } else {
        slot = k;
        w = k + 1;
    }
    FldMenuWinTextAdd(0xA, fldMenuItem.page, sysGrxsysPac->msg->tbl[0], 0x10, 0x30, 0x14, 0x14, slot,
                      fldMenuItem.category * 2 + 0xC, 8);
    if (fldMenuItem.category != 5) {
        FldMenuWinTextAdd(0xA, fldMenuItem.page, sysGrxsysPac->msg->tbl[0], 0xC6, 0x18, 0x14, 0x14, w, 0x19, 1);
    }
    p = (FldMenuQuad *)FldMenuWinGetQuadBuf(0xA);
    FldMenuBarBuild(p, 8, 0x3E, 0x110, 8, 0xA)->type = 0;
    p = FldMenuWinGetSprBuf(0xA, 0);
    FldMenuWinSetLayer(0xA, 0, &fldMenuTextures[0], p, 128.0f);
    if (fldMenuItem.category == 4 || fldMenuItem.category == 5) {
        if (fldMenuItem.category != 5) {
            p = FldMenuNumberBuild(p, cnt, 1, 0xE8, 0x32, 1.0f, 0.8f, 0);
        }
    } else {
        p = FldMenuFractionBuild(p, cnt, sel, 0xC6, 0x32);
    }
    p->type = 0;

    /* category tabs */
    p = FldMenuWinGetSprBuf(0xA, 1);
    FldMenuWinSetLayer(0xA, 1, &fldMenuTextures[1], p, 128.0f);
    fldMenuItem.blink++;
    if (fldMenuItem.blink >= 2) {
        fldMenuItem.blink = 0;
    }
    for (i = 0; i < 6; i++) {
        p->type = 1;
        id = fldMenuItemTabIcons[i];
        if (fldMenuItem.category == i) {
            p->v[0].r = 0x80;
            p->v[0].g = 0x80;
            p->v[0].b = 0x80;
            p->v[0].a = 0;
            if (fldMenuItem.blink == 0) {
                if (fldMenuItem.bobDir != 0) {
                    if (fldMenuItem.bob[i] >= -3) {
                        fldMenuItem.bob[i]--;
                    } else {
                        fldMenuItem.bobDir = 0;
                    }
                } else if (fldMenuItem.bob[i] < 2) {
                    fldMenuItem.bob[i]++;
                } else {
                    fldMenuItem.bobDir = 1;
                }
            }
        } else {
            p->v[0].r = 0x80 - (fldMenuItem.bob[i] + 4) * 9;
            p->v[0].g = 0x80 - (fldMenuItem.bob[i] + 4) * 9;
            p->v[0].b = 0x80 - (fldMenuItem.bob[i] + 4) * 9;
            p->v[0].a = 0;
            if (fldMenuItem.blink == 0) {
                if (fldMenuItem.bob[i] < 4) {
                    fldMenuItem.bob[i]++;
                }
            }
        }
        p->v[0].x = i * 0x1C + (s16)(fldMenuItem.bob[i] / 2 + 8);
        p->v[0].y = fldMenuItem.bob[i] + 8;
        p->v[1].x = i * 0x1C + (menuItemIcons[id].w - fldMenuItem.bob[i]) + (s16)(fldMenuItem.bob[i] / 2 + 8);
        p->v[1].y = fldMenuItem.bob[i] + (s16)(menuItemIcons[id].h - fldMenuItem.bob[i] + 8);
        p->v[0].u = menuItemIcons[id].u0;
        p->v[0].v = menuItemIcons[id].v0;
        p->v[1].u = menuItemIcons[id].u1;
        p->v[1].v = menuItemIcons[id].v1;
        p->z = 0;
        p++;
    }
    p->type = 0;
    return 2;
}
