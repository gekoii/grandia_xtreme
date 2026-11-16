/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclCtrlGenColorPage(PtclCtrl *cfg, PtclGenDef *o) {
    Pad *pad;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 21;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 22) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        switch (sysState.dbgCursor) {
        case 1:
            o->colorC1 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 2:
            o->colorC2 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 3:
            o->colorCurve[0] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 4:
            o->colorCurve[1] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 5:
            o->colorCurve[2] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 6:
            o->colorKeys[0] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 7:
            o->colorKeys[1] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 8:
            o->colorKeys[2] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 9:
            o->colorKeys[3] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 10:
            o->colorKeys[4] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 11:
            o->colorKeys[5] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 12:
            o->colorKeys[6] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 13:
            o->colorKeys[7] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 14:
            o->colorKeys[8] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 15:
            o->colorKeys[9] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 16:
            o->colorKeys[10] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 17:
            o->colorKeys[11] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 18:
            o->colorKeys[12] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 19:
            o->colorKeys[13] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 20:
            o->colorKeys[14] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 21:
            o->colorKeys[15] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        }
    }
    if (sysState.dbgCursor < 6) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 3);
    }
    if (!(cfg->flags & 1)) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(3, 3, dbgPtclC1dFmt, o->colorC1);
    DbgPrintf(3, 4, dbgPtclC2dFmt, o->colorC2);
    DbgPrintf(3, 5, dbgPtclStC1Fmt, o->colorCurve[0]);
    DbgPrintf(3, 6, dbgPtclC1C2Fmt, o->colorCurve[1]);
    DbgPrintf(3, 7, dbgPtclC2EdFmt, o->colorCurve[2]);
    if (!(cfg->genFlags & 0x100)) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(23, 3, dbgPtclStRFmt, o->colorKeys[0] + 128);
    DbgPrintf(26, 4, dbgPtclColGFmt, o->colorKeys[1] + 128);
    DbgPrintf(26, 5, dbgPtclColBFmt, o->colorKeys[2] + 128);
    DbgPrintf(26, 6, dbgPtclColAFmt, o->colorKeys[3] + 128);
    DbgPrintf(23, 7, dbgPtclC1RFmt, o->colorKeys[4] + 128);
    DbgPrintf(26, 8, dbgPtclColGFmt, o->colorKeys[5] + 128);
    DbgPrintf(26, 9, dbgPtclColBFmt, o->colorKeys[6] + 128);
    DbgPrintf(26, 10, dbgPtclColAFmt, o->colorKeys[7] + 128);
    DbgPrintf(23, 11, dbgPtclC2RFmt, o->colorKeys[8] + 128);
    DbgPrintf(26, 12, dbgPtclColGFmt, o->colorKeys[9] + 128);
    DbgPrintf(26, 13, dbgPtclColBFmt, o->colorKeys[10] + 128);
    DbgPrintf(26, 14, dbgPtclColAFmt, o->colorKeys[11] + 128);
    DbgPrintf(23, 15, dbgPtclEdRFmt, o->colorKeys[12] + 128);
    DbgPrintf(26, 16, dbgPtclColGFmt, o->colorKeys[13] + 128);
    DbgPrintf(26, 17, dbgPtclColBFmt, o->colorKeys[14] + 128);
    DbgPrintf(26, 18, dbgPtclColAFmt, o->colorKeys[15] + 128);
    DbgPrintf(0, 0, dbgPtclColor0Str);
}
