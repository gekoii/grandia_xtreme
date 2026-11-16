#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"
#include "main/n01.h"

extern s32 dbgPtclTrackEffeStr;
extern s32 dbgPtclTrackPxStr;
extern s32 dbgPtclTrackPyStr;
extern s32 dbgPtclTrackPzStr;
extern s32 dbgPtclTrackRxStr;
extern s32 dbgPtclTrackRyStr;
extern s32 dbgPtclTrackRzStr;
extern s32 dbgPtclTrackAmbStr;
extern s32 dbgPtclTrackAxStr;
extern s32 dbgPtclTrackAyStr;
extern s32 dbgPtclTrackSpdStr;
extern s32 dbgPtclTrackAccStr;
extern s32 dbgPtclTrackLifeStr;
extern s32 dbgPtclTrackProbStr;
extern s32 dbgPtclTrackBAxStr;
extern s32 dbgPtclTrackBAyStr;
extern s32 dbgPtclTrackBRStr;

void DbgPtclCtrlEmitTrackPage(u8 *arg0) {
    s32 active;
    Pad *pad;

    active = 0;
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 17;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 18) {
                    sysState.dbgCursor = 0;
                }
            }
            if (arg0 == NULL) {
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
                DbgPtclEditTrack3(arg0, arg0 + 0x18);
                break;
            case 2:
                DbgPtclEditTrack4(arg0, arg0 + 0x20);
                break;
            case 3:
                DbgPtclEditTrack4(arg0, arg0 + 0x30);
                break;
            case 4:
                DbgPtclEditTrack4(arg0, arg0 + 0x40);
                break;
            case 5:
                DbgPtclEditTrack4(arg0, arg0 + 0x50);
                break;
            case 6:
                DbgPtclEditTrack4(arg0, arg0 + 0x60);
                break;
            case 7:
                DbgPtclEditTrack4(arg0, arg0 + 0x70);
                break;
            case 8:
                DbgPtclEditTrack4(arg0, arg0 + 0x80);
                break;
            case 9:
                DbgPtclEditTrack4(arg0, arg0 + 0x90);
                break;
            case 10:
                DbgPtclEditTrack4(arg0, arg0 + 0xA0);
                break;
            case 11:
                DbgPtclEditTrack4(arg0, arg0 + 0xB0);
                break;
            case 12:
                DbgPtclEditTrack4(arg0, arg0 + 0xC0);
                break;
            case 13:
                DbgPtclEditTrack4(arg0, arg0 + 0xD0);
                break;
            case 14:
                DbgPtclEditTrack4(arg0, arg0 + 0xE0);
                break;
            case 15:
                DbgPtclEditTrack4(arg0, arg0 + 0xF0);
                break;
            case 16:
                DbgPtclEditTrack4(arg0, arg0 + 0x100);
                break;
            case 17:
                DbgPtclEditTrack4(arg0, arg0 + 0x110);
                break;
            }
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    if (arg0 != NULL) {
        if (active) {
            DbgPrintf(sysState.dbgDigit * 4 + 5, sysState.dbgCursor + 2, dbgPtclEditBoxFmt, dbgPtclColor2Str,
                          ((u32)(sysState.loopFrameCount & 0x1F) < 0x10) ? dbgPtclEditBoxStr : dbgPtclEditBoxBlinkStr, dbgPtclColor0Str);
        }
        DbgPtclPrintTrack3(&dbgPtclTrackEffeStr, arg0 + 0x18, 3, 3);
        DbgPtclPrintTrack4(&dbgPtclTrackPxStr, arg0 + 0x20, 3, 4);
        DbgPtclPrintTrack4(&dbgPtclTrackPyStr, arg0 + 0x30, 3, 5);
        DbgPtclPrintTrack4(&dbgPtclTrackPzStr, arg0 + 0x40, 3, 6);
        DbgPtclPrintTrack4(&dbgPtclTrackRxStr, arg0 + 0x50, 3, 7);
        DbgPtclPrintTrack4(&dbgPtclTrackRyStr, arg0 + 0x60, 3, 8);
        DbgPtclPrintTrack4(&dbgPtclTrackRzStr, arg0 + 0x70, 3, 9);
        DbgPtclPrintTrack4(&dbgPtclTrackAmbStr, arg0 + 0x80, 3, 10);
        DbgPtclPrintTrack4(&dbgPtclTrackAxStr, arg0 + 0x90, 3, 11);
        DbgPtclPrintTrack4(&dbgPtclTrackAyStr, arg0 + 0xA0, 3, 12);
        DbgPtclPrintTrack4(&dbgPtclTrackSpdStr, arg0 + 0xB0, 3, 13);
        DbgPtclPrintTrack4(&dbgPtclTrackAccStr, arg0 + 0xC0, 3, 14);
        DbgPtclPrintTrack4(&dbgPtclTrackLifeStr, arg0 + 0xD0, 3, 15);
        DbgPtclPrintTrack4(&dbgPtclTrackProbStr, arg0 + 0xE0, 3, 16);
        DbgPtclPrintTrack4(&dbgPtclTrackBAxStr, arg0 + 0xF0, 3, 17);
        DbgPtclPrintTrack4(&dbgPtclTrackBAyStr, arg0 + 0x100, 3, 18);
        DbgPtclPrintTrack4(&dbgPtclTrackBRStr, arg0 + 0x110, 3, 19);
    }
}
