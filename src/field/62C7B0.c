#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "field/menu.h"
#include "field/n23.h"

FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);
s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuPartyOrderUpdate(void) {
    s16 ids[4];
    s32 i;
    s32 n;
    s32 lv;
    s32 tgt;
    s32 r;
    FldMenuQuad *s1;
    FldMenuQuad *s2;
    FldMenuQuad *s3;
    u16 trig;
    u8 *p;

    n = 0;
    for (i = 0; i < 4; i++) {
        if (FldMenuWinGetState(i + 9) == 0) {
            lv = fldMenuPartyOrder.slots[i];
            if (lv != 0) {
                lv--;
                FldMenuWinOpen(i + 9, i * 0xC + 0xDC, i * 0x5A + 0x70, 0x15E, 0x60, 0, 0, 0x108010);
                FldMenuWinSetPortrait(i + 9, i, (lv + 1) * 100, 0, 0);
            } else {
                FldMenuWinOpen(i + 9, i * 0xC + 0xDC, i * 0x5A + 0x70, 0x15E, 0x60, 0, 0, 0x8010);
            }
            FldMenuPartyOrderSlotsBuild();
        }
    }

    s1 = FldMenuWinGetSprBuf(8, 1);
    FldMenuWinSetLayer(8, 7, &fldMenuTextures[6], s1, 128.0f);
    s2 = FldMenuWinGetSprBuf(8, 2);
    FldMenuWinSetLayer(8, 1, &fldMenuTextures[2], s2, 128.0f);
    s3 = FldMenuWinGetSprBuf(8, 3);
    FldMenuWinSetLayer(8, 2, &fldMenuTextures[0], s3, 128.0f);
    FldMenuWinTextClear(8);

    for (i = 0; i < fldMenuPartyOrder.count; i++) {
        s1->type = 1;
        if (FldMenuPartyOrderHasChar(i + 2) != 0) {
            tgt = 0x20;
            s1->v[0].a = 0;
            s1->v[0].r = 0x20;
            s1->v[0].g = 0x20;
            s1->v[0].b = 0x20;
        } else {
            s1->v[0].a = 0;
            s1->v[0].r = 0x80;
            s1->v[0].g = 0x80;
            s1->v[0].b = 0x80;
            tgt = 0;
        }
        if (fldMenuPartyOrder.rowX[i] != tgt) {
            if (tgt < fldMenuPartyOrder.rowX[i]) {
                fldMenuPartyOrder.rowX[i] -= 6;
                if (fldMenuPartyOrder.rowX[i] < 0) {
                    fldMenuPartyOrder.rowX[i] = 0;
                }
            } else {
                fldMenuPartyOrder.rowX[i] += 6;
                if (tgt < fldMenuPartyOrder.rowX[i]) {
                    fldMenuPartyOrder.rowX[i] = tgt;
                }
            }
        }
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[2], fldMenuPartyOrder.rowX[i] + 0x28, i * 0x38 + 0x28, 0x10, 0x10, n++, i + 1, tgt / 16 + 1);
        s1->v[0].x = fldMenuPartyOrder.rowX[i] + 8;
        s1->v[0].y = i * 0x38 + 8;
        s1->v[1].x = fldMenuPartyOrder.rowX[i] + 0x38;
        s1->v[1].y = i * 0x38 + 0x38;
        s1->z = 0;
        if (i < 3) {
            s1->v[0].u = i * 0x36 + 0x36;
            s1->v[0].v = 0;
            s1->v[1].u = i * 0x36 + 0x6C;
            s1->v[1].v = 0x36;
        } else {
            s1->v[0].u = (i - 3) * 0x36;
            s1->v[0].v = 0x36;
            s1->v[1].u = (i - 3) * 0x36 + 0x36;
            s1->v[1].v = 0x6C;
        }
        s1++;

        s2->type = 1;
        s2->v[0].r = 0x80;
        s2->v[0].g = 0x80;
        s2->v[0].b = 0x80;
        s2->v[0].a = 0x80;
        s2->v[0].x = fldMenuPartyOrder.rowX[i] + 0x40;
        s2->v[0].y = i * 0x38 + 0x10;
        s2->v[1].x = fldMenuPartyOrder.rowX[i] + 0x5E;
        s2->v[1].y = i * 0x38 + 0x24;
        s2->z = 0;
        s2->v[0].u = 0xEC;
        s2->v[0].v = 0x5C;
        s2->v[1].u = 0x100;
        s2->v[1].v = 0x6A;
        s2++;
        s3 = ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(s3, PtyCharCalcLevel(i + 1), 0, fldMenuPartyOrder.rowX[i] + 0x68, i * 0x38 + 0x12, 0, 1.0f, 0.8f);
    }
    s1->type = 0;
    s2->type = 0;
    s3->type = 0;

    switch (fldMenuPartyOrder.state) {
    case 0:
        if (fldMenuPartyOrder.sub != 0) {
            trig = padPort0[0].trigger;
            if (trig & 0x40) {
                fldMenuPartyOrder.sub = 0;
            } else if (trig & 0x20) {
                if (FldMenuPartyOrderHasChar(fldMenuPartyOrder.sel + 2) == 0) {
                    fldMenuPartyOrder.slots[fldMenuPartyOrder.cursor] = fldMenuPartyOrder.sel + 2;
                    FldMenuWinClose(fldMenuPartyOrder.cursor + 9, 0, 1);
                    fldMenuPartyOrder.sub = 0;
                    SndSePlay(0x7D5, 0, 0, 0x1E, 0);
                }
            } else if (trig & 0x1000) {
                fldMenuPartyOrder.sel = (fldMenuPartyOrder.sel != 0 ? fldMenuPartyOrder.sel : fldMenuPartyOrder.count) - 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            } else if (trig & 0x4000) {
                if (fldMenuPartyOrder.sel < fldMenuPartyOrder.count - 1) {
                    fldMenuPartyOrder.sel++;
                } else {
                    fldMenuPartyOrder.sel = 0;
                }
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            } else {
                FldMenuCursorSetAt(8, 0.0f, (f32)(fldMenuPartyOrder.sel * 0x38) + 32.0f);
            }
        } else if (padPort0[0].trigger & 0x1000) {
            if (fldMenuPartyOrder.cursor != 0) {
                fldMenuPartyOrder.cursor--;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        } else if (padPort0[0].trigger & 0x4000) {
            if (fldMenuPartyOrder.cursor < 3) {
                fldMenuPartyOrder.cursor++;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        } else if (padPort0[0].trigger & 0x40) {
            r = FldMenuPartyOrderIsComplete(0);
            if (r == 1) {
                FldMenuWinOpen(0xD, 0x140, 0xE0, 0x118, 0x80, 0, 1, 0x8018);
                fldMenuPartyOrder.state = r;
                fldMenuPartyOrder.yesno = r;
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                FldMenuWinSetActive(0xD);
            } else {
                SndSePlay(0x7D4, 0, 0, 0x1E, 0);
            }
        } else {
            FldMenuCursorSetAt(fldMenuPartyOrder.cursor + 9, 0.0f, 32.0f);
            FldMenuWinSetActive(fldMenuPartyOrder.cursor + 9);
            if (padPort0[0].trigger & 0x20) {
                p = &fldMenuPartyOrder.slots[fldMenuPartyOrder.cursor];
                if (*p != 0) {
                    if (fldMenuPartyOrder.cursor != 0) {
                        *p = 0;
                        FldMenuWinClose(fldMenuPartyOrder.cursor + 9, 0, 1);
                    }
                    SndSePlay(0x7E0, 0, 0, 0x1E, 0);
                } else {
                    fldMenuPartyOrder.sub = 1;
                    SndSePlay(0x7D5, 0, 0, 0x1E, 0);
                }
            }
        }
        break;
    case 1:
        FldMenuWinTextClear(0xD);
        FldMenuWinTextAdd(0xD, 1, sysGrxsysPac->msg->tbl[0], 0x10, 0x10, 0x14, 0x14, n++, 0x28, 0);
        FldMenuWinTextAdd(0xD, 1, sysGrxsysPac->msg->tbl[0], 0x10, 0x24, 0x14, 0x14, n++, 0x29, 0);
        if (fldMenuPartyOrder.yesno == 0) {
            FldMenuWinTextAdd(0xD, 1, sysGrxsysPac->msg->tbl[0], 0x76, 0x42, 0x14, 0x14, n++, 0x2A, 8);
            FldMenuWinTextAdd(0xD, 1, sysGrxsysPac->msg->tbl[0], 0x76, 0x5A, 0x14, 0x14, n++, 0x2B, 0);
        } else {
            FldMenuWinTextAdd(0xD, 1, sysGrxsysPac->msg->tbl[0], 0x76, 0x42, 0x14, 0x14, n++, 0x2A, 0);
            FldMenuWinTextAdd(0xD, 1, sysGrxsysPac->msg->tbl[0], 0x76, 0x5A, 0x14, 0x14, n++, 0x2B, 8);
        }
        if ((padPort0[0].trigger & 0x1000) && fldMenuPartyOrder.yesno != 0) {
            fldMenuPartyOrder.yesno = 0;
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        } else if ((padPort0[0].trigger & 0x4000) && fldMenuPartyOrder.yesno == 0) {
            fldMenuPartyOrder.yesno = 1;
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        } else {
            FldMenuCursorSetAt(0xD, 118.0f - (f32)(fldMenuPartyOrder.yesno * 10), (f32)(fldMenuPartyOrder.yesno * 0x18) + 74.0f);
            if (padPort0[0].trigger & 0x40) {
                fldMenuPartyOrder.state = 0;
                FldMenuWinClose(0xD, 0, 1);
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                FldMenuWinSetActive(fldMenuPartyOrder.cursor + 9);
            } else if (padPort0[0].trigger & 0x20) {
                if (fldMenuPartyOrder.yesno == 0) {
                    FldMenuClose();
                    ids[0] = fldMenuPartyOrder.slots[0];
                    ids[1] = fldMenuPartyOrder.slots[1];
                    ids[2] = fldMenuPartyOrder.slots[2];
                    ids[3] = fldMenuPartyOrder.slots[3];
                    PtyPartySet(ids);
                    PtyPartyGet(ids);
                    fldMenuPartyOrder.slots[0] = ids[0];
                    fldMenuPartyOrder.slots[1] = ids[1];
                    fldMenuPartyOrder.slots[2] = ids[2];
                    fldMenuPartyOrder.slots[3] = ids[3];
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                } else {
                    fldMenuPartyOrder.state = 0;
                    FldMenuWinClose(0xD, 0, 1);
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                    FldMenuWinSetActive(fldMenuPartyOrder.cursor + 9);
                }
            }
        }
        break;
    }
    return 4;
}
