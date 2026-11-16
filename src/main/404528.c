#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "main/n01.h"

extern u8 dbgPtclTrackRdxStr[];
extern u8 dbgPtclTrackRdxMdStr[];
extern u8 dbgPtclTrackRdyStr[];
extern u8 dbgPtclTrackRdyMdStr[];
extern u8 dbgPtclTrackRdzStr[];
extern u8 dbgPtclTrackRdzMdStr[];

void DbgPtclCtrlRotTrackPage(u8 *arg0) {
    s32 flag;
    Pad *pad;

    flag = 0;
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 6;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 7) {
                    sysState.dbgCursor = 0;
                }
            }
            if (arg0 == 0) {
                sysState.dbgCursor = 0;
            }
        }
        if (pad->held & 0x20) {
            flag = 1;
            switch (sysState.dbgCursor) {
            case 0:
                flag = 0;
                break;
            case 1:
                DbgPtclEditTrack4(arg0, arg0 + 0x1E8);
                break;
            case 2:
                DbgPtclEditTrack3(arg0, arg0 + 0x1F8);
                break;
            case 3:
                DbgPtclEditTrack4(arg0, arg0 + 0x200);
                break;
            case 4:
                DbgPtclEditTrack3(arg0, arg0 + 0x210);
                break;
            case 5:
                DbgPtclEditTrack4(arg0, arg0 + 0x218);
                break;
            case 6:
                DbgPtclEditTrack3(arg0, arg0 + 0x228);
                break;
            }
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    if (arg0 != 0) {
        if (flag != 0) {
            DbgPrintf(sysState.dbgDigit * 4 + 5, sysState.dbgCursor + 2, dbgPtclEditBoxFmt, dbgPtclColor2Str,
                          (u32)(sysState.loopFrameCount & 0x1F) < 0x10 ? dbgPtclEditBoxStr : dbgPtclEditBoxBlinkStr, dbgPtclColor0Str);
        }
        DbgPtclPrintTrack4(dbgPtclTrackRdxStr, arg0 + 0x1E8, 3, 3);
        DbgPtclPrintTrack3(dbgPtclTrackRdxMdStr, arg0 + 0x1F8, 3, 4);
        DbgPtclPrintTrack4(dbgPtclTrackRdyStr, arg0 + 0x200, 3, 5);
        DbgPtclPrintTrack3(dbgPtclTrackRdyMdStr, arg0 + 0x210, 3, 6);
        DbgPtclPrintTrack4(dbgPtclTrackRdzStr, arg0 + 0x218, 3, 7);
        DbgPtclPrintTrack3(dbgPtclTrackRdzMdStr, arg0 + 0x228, 3, 8);
    }
}
