#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"
#include "main/n01.h"

extern char dbgPtclTrackSxSinStr[];
extern char dbgPtclTrackSxMdStr[];
extern char dbgPtclTrackSxCtStr[];
extern char dbgPtclTrackSx0Str[];
extern char dbgPtclTrackSxRStr[];
extern char dbgPtclTrackSySinStr[];
extern char dbgPtclTrackSyMdStr[];
extern char dbgPtclTrackSyCtStr[];
extern char dbgPtclTrackSy0Str[];
extern char dbgPtclTrackSyRStr[];
extern char dbgPtclTrackVcMdStr[];
extern char dbgPtclTrackVcCtStr[];
extern char dbgPtclTrackVcolStr[];
extern char dbgPtclTrackOnceStr[];

void DbgPtclCtrlScaleTrackPage(u8 *arg0) {
    SysState *p;
    s32 active;
    Pad *pad;

    active = 0;
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 14;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 15) {
                    sysState.dbgCursor = 0;
                }
            }
            if (arg0 == 0) {
                sysState.dbgCursor = 0;
            }
        }
        if (pad->held & 0x20) {
            active = 1;
            switch (sysState.dbgCursor) {
            case 0:
                active = 0;
                break;
            case 1:
                DbgPtclEditTrack4(arg0, arg0 + 0x130);
                break;
            case 2:
                DbgPtclEditTrack3(arg0, arg0 + 0x140);
                break;
            case 3:
                DbgPtclEditTrack4(arg0, arg0 + 0x148);
                break;
            case 4:
                DbgPtclEditTrack4(arg0, arg0 + 0x158);
                break;
            case 5:
                DbgPtclEditTrack4(arg0, arg0 + 0x168);
                break;
            case 6:
                DbgPtclEditTrack4(arg0, arg0 + 0x178);
                break;
            case 7:
                DbgPtclEditTrack3(arg0, arg0 + 0x188);
                break;
            case 8:
                DbgPtclEditTrack4(arg0, arg0 + 0x190);
                break;
            case 9:
                DbgPtclEditTrack4(arg0, arg0 + 0x1A0);
                break;
            case 10:
                DbgPtclEditTrack4(arg0, arg0 + 0x1B0);
                break;
            case 11:
                DbgPtclEditTrack3(arg0, arg0 + 0x1C0);
                break;
            case 12:
                DbgPtclEditTrack4(arg0, arg0 + 0x1C8);
                break;
            case 13:
                DbgPtclEditTrack4(arg0, arg0 + 0x1D8);
                break;
            case 14:
                DbgPtclEditTrack4(arg0, arg0 + 0x120);
                break;
            }
        }
    }
    p = &sysState;
    DbgDrawCursor(2, p->dbgCursor + 2);
    if (arg0 != 0) {
        if (active) {
            DbgPrintf(p->dbgDigit * 4 + 5, p->dbgCursor + 2, dbgPtclEditBoxFmt, dbgPtclColor2Str,
                          (p->loopFrameCount & 0x1F) < 0x10 ? dbgPtclEditBoxStr : dbgPtclEditBoxBlinkStr, dbgPtclColor0Str);
        }
        DbgPtclPrintTrack4(dbgPtclTrackSxSinStr, arg0 + 0x130, 3, 3);
        DbgPtclPrintTrack3(dbgPtclTrackSxMdStr, arg0 + 0x140, 3, 4);
        DbgPtclPrintTrack4(dbgPtclTrackSxCtStr, arg0 + 0x148, 3, 5);
        DbgPtclPrintTrack4(dbgPtclTrackSx0Str, arg0 + 0x158, 3, 6);
        DbgPtclPrintTrack4(dbgPtclTrackSxRStr, arg0 + 0x168, 3, 7);
        DbgPtclPrintTrack4(dbgPtclTrackSySinStr, arg0 + 0x178, 3, 8);
        DbgPtclPrintTrack3(dbgPtclTrackSyMdStr, arg0 + 0x188, 3, 9);
        DbgPtclPrintTrack4(dbgPtclTrackSyCtStr, arg0 + 0x190, 3, 0xA);
        DbgPtclPrintTrack4(dbgPtclTrackSy0Str, arg0 + 0x1A0, 3, 0xB);
        DbgPtclPrintTrack4(dbgPtclTrackSyRStr, arg0 + 0x1B0, 3, 0xC);
        DbgPtclPrintTrack3(dbgPtclTrackVcMdStr, arg0 + 0x1C0, 3, 0xD);
        DbgPtclPrintTrack4(dbgPtclTrackVcCtStr, arg0 + 0x1C8, 3, 0xE);
        DbgPtclPrintTrack4(dbgPtclTrackVcolStr, arg0 + 0x1D8, 3, 0xF);
        DbgPtclPrintTrack4(dbgPtclTrackOnceStr, arg0 + 0x120, 3, 0x10);
    }
}
