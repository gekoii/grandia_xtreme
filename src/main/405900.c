/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclCtrlGenScalePage(PtclCtrl *cfg, PtclGenDef *o) {
    Pad *pad;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 36;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 37) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        switch (sysState.dbgCursor) {
        case 1:
            o->sx[0] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 2:
            o->sx[1] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 3:
            o->sx[2] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 4:
            o->sx[3] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 5:
            o->sx[4] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 6:
            o->sx[5] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 7:
            o->sx[6] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 8:
            o->sx[7] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 9:
            o->sx[8] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 10:
            o->sx[9] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 11:
            o->sxC1 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 12:
            o->sxC2 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 13:
            o->sxCurve[0] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 14:
            o->sxCurve[1] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 15:
            o->sxCurve[2] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 16:
            o->sy[0] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 17:
            o->sy[1] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 18:
            o->sy[2] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 19:
            o->sy[3] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 20:
            o->sy[4] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 21:
            o->sy[5] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 22:
            o->sy[6] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 23:
            o->sy[7] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 24:
            o->sy[8] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 25:
            o->sy[9] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 26:
            o->syC1 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 27:
            o->syC2 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 28:
            o->syCurve[0] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 29:
            o->syCurve[1] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 30:
            o->syCurve[2] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 31:
            o->sinXLen += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 32:
            o->sinXStart += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 33:
            o->sinXRate += DbgEditValue(2, -2, 0, 0.1f);
            break;
        case 34:
            o->sinYLen += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 35:
            o->sinYStart += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 36:
            o->sinYRate += DbgEditValue(2, -2, 0, 0.1f);
            break;
        }
    }
    if (sysState.dbgCursor < 16) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else if (sysState.dbgCursor < 31) {
        DbgDrawCursor(22, sysState.dbgCursor - 13);
    } else {
        DbgDrawCursor(37, sysState.dbgCursor - 28);
    }
    DbgPrintf(3, 3, dbgPtclSx0Fmt, o->sx[0]);
    DbgPrintf(11, 4, dbgPtclVal51fFmt, o->sx[1]);
    DbgPrintf(3, 5, dbgPtclRateStFmt, o->sx[2]);
    DbgPrintf(11, 6, dbgPtclVal51fFmt, o->sx[3]);
    DbgPrintf(8, 7, dbgPtclC1fFmt, o->sx[4]);
    DbgPrintf(11, 8, dbgPtclVal51fFmt, o->sx[5]);
    DbgPrintf(8, 9, dbgPtclC2fFmt, o->sx[6]);
    DbgPrintf(11, 10, dbgPtclVal51fFmt, o->sx[7]);
    DbgPrintf(8, 11, dbgPtclEdfFmt, o->sx[8]);
    DbgPrintf(11, 12, dbgPtclVal51fFmt, o->sx[9]);
    DbgPrintf(3, 13, dbgPtclC1dFmt, o->sxC1);
    DbgPrintf(3, 14, dbgPtclC2dFmt, o->sxC2);
    DbgPrintf(3, 15, dbgPtclStC1Fmt, (u8)o->sxCurve[0]);
    DbgPrintf(3, 16, dbgPtclC1C2Fmt, (u8)o->sxCurve[1]);
    DbgPrintf(3, 17, dbgPtclC2EdFmt, (u8)o->sxCurve[2]);
    DbgPrintf(38, 3, dbgPtclSinXLFmt, o->sinXLen);
    if (o->sinXLen <= 0) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(38, 4, dbgPtclStdFmt, o->sinXStart);
    DbgPrintf(38, 5, dbgPtclRateFmt, o->sinXRate);
    if (cfg->genFlags & 0x80) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    } else if (o->sinXLen <= 0) {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
    DbgPrintf(23, 3, dbgPtclSy0Fmt, o->sy[0]);
    DbgPrintf(31, 4, dbgPtclVal51fFmt, o->sy[1]);
    DbgPrintf(23, 5, dbgPtclRateStFmt, o->sy[2]);
    DbgPrintf(31, 6, dbgPtclVal51fFmt, o->sy[3]);
    DbgPrintf(28, 7, dbgPtclC1fFmt, o->sy[4]);
    DbgPrintf(31, 8, dbgPtclVal51fFmt, o->sy[5]);
    DbgPrintf(28, 9, dbgPtclC2fFmt, o->sy[6]);
    DbgPrintf(31, 10, dbgPtclVal51fFmt, o->sy[7]);
    DbgPrintf(28, 11, dbgPtclEdfFmt, o->sy[8]);
    DbgPrintf(31, 12, dbgPtclVal51fFmt, o->sy[9]);
    DbgPrintf(23, 13, dbgPtclC1dFmt, o->syC1);
    DbgPrintf(23, 14, dbgPtclC2dFmt, o->syC2);
    DbgPrintf(23, 15, dbgPtclStC1Fmt, (u8)o->syCurve[0]);
    DbgPrintf(23, 16, dbgPtclC1C2Fmt, (u8)o->syCurve[1]);
    DbgPrintf(23, 17, dbgPtclC2EdFmt, (u8)o->syCurve[2]);
    DbgPrintf(38, 6, dbgPtclSinYLFmt, o->sinYLen);
    if (o->sinYLen <= 0) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(38, 7, dbgPtclStdFmt, o->sinYStart);
    DbgPrintf(38, 8, dbgPtclRateFmt, o->sinYRate);
    if ((cfg->genFlags & 0x80) || o->sinYLen <= 0) {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
}
