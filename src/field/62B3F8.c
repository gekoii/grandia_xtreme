#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "save.h"
#include "field/n23.h"

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuOptionsUpdate(void) {
    s32 i;
    s32 lim;
    s32 k;
    s32 h;
    s32 y;
    s32 n;
    u16 trig;

    lim = 0x2A;
    for (i = 0; i < 7; i++, lim -= 6) {
        if (i == fldMenuOptions.cursor) {
            fldMenuOptions.rowX[i] -= 8;
            if (fldMenuOptions.rowX[i] < lim) {
                fldMenuOptions.rowX[i] = lim;
            }
        } else {
            fldMenuOptions.rowX[i] += 8;
            if (fldMenuOptions.rowX[i] >= lim + 0x10) {
                fldMenuOptions.rowX[i] = lim + 0x10;
            }
        }
    }

    k = 6;
    h = FldMenuWinGetQuadBuf(8);
    y = 8;
    FldMenuWinTextClear(8);
    FldMenuWinTextClear(9);
    n = 0;
    FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], 0x100, 0xC2, 0x18, 0x14, n++, 0x15, k);
    for (i = 0; i < 7; i++) {
        h = FldMenuBarBuild(h, fldMenuOptions.rowX[i] + 8, y + 0x10, 0x140, 0xC, 1);
        if (i == fldMenuOptions.cursor) {
            FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[i] + 0x10, y, 0x16, 0x16, n++, i + 1, 8);
        } else {
            FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[i] + 0x10, y, 0x14, 0x14, n++, i + 1, 0);
        }
        y += 0x18;
    }

    y = 8;
    if (fldMenuOptions.vibration == 0) {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[0] + 0x104, y, 0x14, 0x14, n++, 0xF, 2);
    } else {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[0] + 0x104, y, 0x14, 0x14, n++, 0x10, 2);
    }
    y += 0x18;
    if (fldMenuOptions.camReverse == 0) {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[1] + 0x100, y, 0x14, 0x14, n++, 0x11, 2);
    } else {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[1] + 0x100, y, 0x14, 0x14, n++, 0x12, 2);
    }
    y += 0x18;
    if (fldMenuOptions.soundMode == 0) {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[2] + 0xF8, y, 0x14, 0x14, n++, 0xA, 2);
    } else {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[2] + 0x100, y, 0x14, 0x14, n++, 0xB, 2);
    }
    y += 0x18;
    if (fldMenuOptions.ipStop == 0) {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[3] + 0x100, y, 0x14, 0x14, n++, 0xC, 2);
    } else {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[3] + 0x100, y, 0x14, 0x14, n++, 0xE, 2);
    }
    y += 0x18;
    if (fldMenuOptions.miniMapOff == 0) {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[4] + 0x104, y, 0x14, 0x14, n++, 0xF, 2);
    } else {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[4] + 0x104, y, 0x14, 0x14, n++, 0x10, 2);
    }
    y += 0x18;
    if (fldMenuOptions.miniMap2A == 0) {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[5] + 0x104, y, 0x14, 0x14, n++, 0xF, 2);
    } else {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[5] + 0x104, y, 0x14, 0x14, n++, 0x10, 2);
    }
    y += 0x18;
    if (fldMenuOptions.miniMap2B == 0) {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[6] + 0xF0, y, 0x14, 0x14, n++, 0x13, 2);
    } else {
        FldMenuWinTextAdd(8, 1, sysGrxsysPac->msg->tbl[4], fldMenuOptions.rowX[6] + 0xD8, y, 0x14, 0x14, n++, 0x14, 2);
    }

    switch (fldMenuOptions.state) {
    case 0:
        FldMenuCursorSetAt(8, (f32)((7 - fldMenuOptions.cursor) * 6) + 16.0f, (f32)(fldMenuOptions.cursor * 0x18) + 20.0f);
        trig = padPort0[0].trigger;
        if (trig & 0x20) {
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            FldMenuWinOpen(9, 0x1E0, fldMenuOptions.cursor * 0x1C + 0xC4, 0x80, 0x40, 0, 0, 0x8008);
            FldMenuWinSetActive(9);
            fldMenuOptions.state = 1;
            switch (fldMenuOptions.cursor) {
            case 0: fldMenuOptions.value = fldMenuOptions.vibration; break;
            case 1: fldMenuOptions.value = fldMenuOptions.camReverse; break;
            case 2: fldMenuOptions.value = fldMenuOptions.soundMode; break;
            case 3: fldMenuOptions.value = fldMenuOptions.ipStop; break;
            case 4: fldMenuOptions.value = fldMenuOptions.miniMapOff; break;
            case 5: fldMenuOptions.value = fldMenuOptions.miniMap2A; break;
            case 6: fldMenuOptions.value = fldMenuOptions.miniMap2B; break;
            }
        } else if (trig & 0x1000) {
            if (fldMenuOptions.cursor != 0) {
                fldMenuOptions.cursor--;
            } else {
                fldMenuOptions.cursor = 6;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (trig & 0x4000) {
            if (fldMenuOptions.cursor < 6) {
                fldMenuOptions.cursor++;
            } else {
                fldMenuOptions.cursor = 0;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        } else if (trig & 0x40) {
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            sysSaveData.optVibration = fldMenuOptions.vibration;
            sysSaveData.optCamReverse = fldMenuOptions.camReverse;
            sysSaveData.optSoundMode = fldMenuOptions.soundMode;
            sysSaveData.optIpStop = fldMenuOptions.ipStop;
            sysSaveData.optMiniMapOff = fldMenuOptions.miniMapOff;
            sysSaveData.optMiniMap2A = fldMenuOptions.miniMap2A;
            sysSaveData.optMiniMap2B = fldMenuOptions.miniMap2B;
            if (FldMenuIsDirectOpen() == 1) {
                FldMenuClose();
            } else {
                if (sysSaveData.optVibration == 0) {
                    PadSetVibEnabled(1);
                } else {
                    PadSetVibEnabled(0);
                }
                return 1;
            }
        }
        break;
    case 1:
        FldMenuCursorSetAt(9, 8.0f, (f32)(fldMenuOptions.value * 0x18) + 16.0f);
        n += 2;
        switch (fldMenuOptions.cursor) {
        case 1:
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x20, 4, 0x18, 0x18, n++, 0x11, 2);
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x20, 0x20, 0x18, 0x18, n++, 0x12, 2);
            break;
        case 2:
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x18, 4, 0x18, 0x18, n++, 0xA, 2);
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x18, 0x20, 0x18, 0x18, n++, 0xB, 2);
            break;
        case 3:
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x20, 4, 0x18, 0x18, n++, 0xC, 2);
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x20, 0x20, 0x18, 0x18, n++, 0xE, 2);
            break;
        case 0:
        case 4:
        case 5:
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x28, 4, 0x18, 0x18, n++, 0xF, 2);
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 0x28, 0x20, 0x18, 0x18, n++, 0x10, 2);
            break;
        case 6:
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 8, 4, 0x14, 0x18, n++, 0x13, 2);
            FldMenuWinTextAdd(9, 1, sysGrxsysPac->msg->tbl[4], 8, 0x20, 0x14, 0x18, n++, 0x14, 2);
            break;
        }
        if (padPort0[0].trigger & 0x40) {
            FldMenuWinClose(9, 0, 3);
            FldMenuWinSetActive(8);
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            fldMenuOptions.state = 0;
        } else if (padPort0[0].trigger & 0x4000) {
            if (fldMenuOptions.value == 0) {
                fldMenuOptions.value = 1;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            }
        } else if (padPort0[0].trigger & 0x1000) {
            if (fldMenuOptions.value != 0) {
                fldMenuOptions.value = 0;
                SndSePlay(0x7D1, 0, 0, 0x1E, 0);
                if (fldMenuOptions.cursor == 0) {
                    PadSetVibEnabled(1);
                    PadVibrate(padPort0, 1, 0x80, 4);
                }
            }
        } else if (padPort0[0].trigger & 0x20) {
            FldMenuWinClose(9, 0, 3);
            FldMenuWinSetActive(8);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            fldMenuOptions.state = 0;
            switch (fldMenuOptions.cursor) {
            case 0: fldMenuOptions.vibration = fldMenuOptions.value; break;
            case 1: fldMenuOptions.camReverse = fldMenuOptions.value; break;
            case 2: fldMenuOptions.soundMode = fldMenuOptions.value; break;
            case 3: fldMenuOptions.ipStop = fldMenuOptions.value; break;
            case 4: fldMenuOptions.miniMapOff = fldMenuOptions.value; break;
            case 5: fldMenuOptions.miniMap2A = fldMenuOptions.value; break;
            case 6: fldMenuOptions.miniMap2B = fldMenuOptions.value; break;
            }
            if (fldMenuOptions.cursor == 2) {
                SndSetOutputMode(fldMenuOptions.soundMode);
            }
        }
        break;
    }
    return 3;
}
