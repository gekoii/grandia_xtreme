/* rodata: 0x00675310-0x00675320 */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "save.h"
#include "field/menu.h"
#include "field/n23.h"

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuTacticsUpdate(void) {
    u8 cols[2][2] = { { 8, 0 }, { 0, 8 } };
    s32 count;
    s32 h;
    s32 i;
    s32 n;
    s32 v;
    FldMenuQuad *s;
    u16 *tbl;
    u16 trig;

    n = 0;
    FldMenuWinTextClear(8);
    FldMenuWinTextClear(9);
    s = FldMenuGetSprBuf(4);
    count = 0;
    FldMenuWinSetLayer(8, 7, &fldMenuTexFaces, s, 128.0f);
    h = FldMenuWinGetQuadBuf(8);
    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0x150, 0xF2, 0x18, 0x14, n++, 1, 6);
    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0xC8, 4, 0x14, 0x14, n++, 2, 2);
    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0x154, 4, 0x14, 0x14, n++, 3, 2);
    tbl = sysSaveParty;
    for (i = 0; i < 4; i++) {
        v = tbl[i];
        if (v != 0) {
            v--;
            FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[2], 0x28, i * 0x38 + 0x28, 0x10, 0x10, n++, v, 1);
            if (fldMenuTactics.row == i) {
                if (fldMenuTactics.tactics[i][0] != 0) {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0xB4, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][0] + 4, cols[fldMenuTactics.col][0]);
                } else {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0xA8, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][0] + 4, cols[fldMenuTactics.col][0]);
                }
                if (fldMenuTactics.tactics[i][1] != 0) {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0x140, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][1] + 4, cols[fldMenuTactics.col][1]);
                } else {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0x134, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][1] + 4, cols[fldMenuTactics.col][1]);
                }
            } else {
                if (fldMenuTactics.tactics[i][0] != 0) {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0xB4, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][0] + 4, 0);
                } else {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0xA8, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][0] + 4, 0);
                }
                if (fldMenuTactics.tactics[i][1] != 0) {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0x140, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][1] + 4, 0);
                } else {
                    FldMenuWinTextAdd(8, 0, sysGrxsysPac->msg->tbl[6], 0x134, i * 0x38 + 0x22, 0x14, 0x14, n++, fldMenuTactics.tactics[i][1] + 4, 0);
                }
            }
            h = FldMenuBarBuild(h, 8, i * 0x38 + 0x34, 0x1C0, 0xC, 0xA);
            s->type = 1;
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0;
            s->v[0].x = 8;
            s->v[0].y = i * 0x38 + 8;
            s->v[1].x = 0x38;
            s->v[1].y = i * 0x38 + 0x38;
            s->z = 0;
            if (v >= 4) {
                s->v[0].u = (v - 4) * 0x36;
                s->v[0].v = 0x36;
                s->v[1].u = (v - 4) * 0x36 + 0x36;
                s->v[1].v = 0x6C;
            } else {
                s->v[0].u = v * 0x36;
                s->v[0].v = 0;
                s->v[1].u = v * 0x36 + 0x36;
                s->v[1].v = 0x36;
            }
            s++;
            count++;
        }
    }
    s->type = 0;

    switch (fldMenuTactics.state) {
    case 0:
        FldMenuCursorSetAt(8, (f32)(fldMenuTactics.col * 0x96) + 170.0f, (f32)(fldMenuTactics.row * 0x38) + 40.0f);
        trig = padPort0[0].trigger;
        if (trig & 0x4000) {
            if (fldMenuTactics.row < count - 1) {
                fldMenuTactics.row++;
            } else {
                fldMenuTactics.row = 0;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (trig & 0x1000) {
            fldMenuTactics.row = (fldMenuTactics.row != 0 ? fldMenuTactics.row : count) - 1;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else {
            if ((trig & 0x8000) && fldMenuTactics.col != 0) {
                fldMenuTactics.col = 0;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
            if (padPort0[0].trigger & 0x2000) {
                if (fldMenuTactics.col == 0) {
                    fldMenuTactics.col = 1;
                    SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                }
            }
            if (padPort0[0].trigger & 0x20) {
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                FldMenuWinOpen(9, 0x78, 0xE6, 0x96, 0xE8, 0, 0, 0x8010);
                FldMenuWinSetActive(9);
                fldMenuTactics.state = 1;
                fldMenuTactics.sel = fldMenuTactics.tactics[fldMenuTactics.row][fldMenuTactics.col];
            } else if (padPort0[0].trigger & 0x40) {
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                FldMenuTacticsApply();
                return 1;
            }
        }
        break;
    case 1:
        FldMenuCursorSetAt(9, 8.0f, (f32)(fldMenuTactics.sel * 0x1C) + 20.0f);
        for (i = 0; i < 8; i++) {
            if (fldMenuTactics.sel == i) {
                FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[6], 8, i * 0x1C + 8, 0x18, 0x18, n++, i + 4, 8);
            } else {
                FldMenuWinTextAdd(9, 0, sysGrxsysPac->msg->tbl[6], 8, i * 0x1C + 8, 0x18, 0x18, n++, i + 4, 0);
            }
        }
        trig = padPort0[0].trigger;
        if (trig & 0x4000) {
            if (fldMenuTactics.sel < 7) {
                fldMenuTactics.sel++;
            } else {
                fldMenuTactics.sel = 0;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (trig & 0x1000) {
            fldMenuTactics.sel = (fldMenuTactics.sel != 0 ? fldMenuTactics.sel : 8) - 1;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (trig & 0x20) {
            FldMenuWinSetActive(8);
            FldMenuWinClose(9, 0, 0);
            fldMenuTactics.state = 0;
            fldMenuTactics.tactics[fldMenuTactics.row][fldMenuTactics.col] = fldMenuTactics.sel;
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        } else if (trig & 0x40) {
            FldMenuWinSetActive(8);
            FldMenuWinClose(9, 0, 0);
            fldMenuTactics.state = 0;
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
        }
        break;
    }
    if (fldMenuTactics.state == 0) {
        fldMenuTactics.cur = fldMenuTactics.tactics[fldMenuTactics.row][fldMenuTactics.col];
    } else {
        fldMenuTactics.cur = fldMenuTactics.sel;
    }
    if (FldMenuWinGetState(8) == 4) {
        if (fldMenuTactics.cur != fldMenuTactics.last) {
            fldMenuTactics.last = fldMenuTactics.cur;
            MenuHelpOpen(2, (s8)fldMenuTactics.cur, 0, 16.0f, 360.0f);
        }
    }
    return 5;
}
