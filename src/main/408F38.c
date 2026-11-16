#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclGenScalePage(s32 arg0) {
    PtclGen *u;
    PtclGenDef *p;
    Pad *pad;

    u = ptclMainPool->gens[arg0];
    p = (u != NULL) ? &u->def : NULL;
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 37;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 38) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        if (u == NULL) {
            sysState.dbgCursor = 0;
        }
        switch (sysState.dbgCursor) {
        case 1:
            if (pad->repeat & 0xA000) {
                u->flags ^= 0x80;
            }
            break;
        case 2:
            p->sx[0] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 3:
            p->sx[1] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 4:
            p->sx[2] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 5:
            p->sx[3] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 6:
            p->sx[4] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 7:
            p->sx[5] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 8:
            p->sx[6] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 9:
            p->sx[7] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 10:
            p->sx[8] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 11:
            p->sx[9] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 12:
            p->sxC1 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 13:
            p->sxC2 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 14:
            p->sxCurve[0] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 15:
            p->sxCurve[1] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 16:
            p->sxCurve[2] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 17:
            p->sy[0] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 18:
            p->sy[1] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 19:
            p->sy[2] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 20:
            p->sy[3] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 21:
            p->sy[4] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 22:
            p->sy[5] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 23:
            p->sy[6] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 24:
            p->sy[7] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 25:
            p->sy[8] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 26:
            p->sy[9] += DbgEditValue(2, -1, 3, 1.0f);
            break;
        case 27:
            p->syC1 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 28:
            p->syC2 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 29:
            p->syCurve[0] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 30:
            p->syCurve[1] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 31:
            p->syCurve[2] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 32:
            p->sinXLen += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 33:
            p->sinXStart += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 34:
            p->sinXRate += DbgEditValue(2, -2, 0, 0.1f);
            break;
        case 35:
            p->sinYLen += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 36:
            p->sinYStart += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 37:
            p->sinYRate += DbgEditValue(2, -2, 0, 0.1f);
            break;
        }
    }
    if (sysState.dbgCursor < 17) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else if (sysState.dbgCursor < 32) {
        DbgDrawCursor(22, sysState.dbgCursor - 13);
    } else {
        DbgDrawCursor(37, sysState.dbgCursor - 28);
    }
    if (u != NULL) {
        DbgPrintf(3, 3, dbgPtclSizeLinkFmt, (u->flags & 0x80) ? dbgPtclEqualStr : dbgPtclNotEqualStr);
        DbgPrintf(3, 4, dbgPtclSx0Fmt, (p->sx[0]));
        DbgPrintf(11, 5, dbgPtclVal51fFmt, (p->sx[1]));
        DbgPrintf(3, 6, dbgPtclRateStFmt, (p->sx[2]));
        DbgPrintf(11, 7, dbgPtclVal51fFmt, (p->sx[3]));
        DbgPrintf(8, 8, dbgPtclC1fFmt, (p->sx[4]));
        DbgPrintf(11, 9, dbgPtclVal51fFmt, (p->sx[5]));
        DbgPrintf(8, 10, dbgPtclC2fFmt, (p->sx[6]));
        DbgPrintf(11, 11, dbgPtclVal51fFmt, (p->sx[7]));
        DbgPrintf(8, 12, dbgPtclEdfFmt, (p->sx[8]));
        DbgPrintf(11, 13, dbgPtclVal51fFmt, (p->sx[9]));
        DbgPrintf(3, 14, dbgPtclC1dFmt, p->sxC1);
        DbgPrintf(3, 15, dbgPtclC2dFmt, p->sxC2);
        DbgPrintf(3, 16, dbgPtclStC1Fmt, (u8)p->sxCurve[0]);
        DbgPrintf(3, 17, dbgPtclC1C2Fmt, (u8)p->sxCurve[1]);
        DbgPrintf(3, 18, dbgPtclC2EdFmt, (u8)p->sxCurve[2]);
        DbgPrintf(38, 4, dbgPtclSinXLFmt, p->sinXLen);
        if (p->sinXLen <= 0) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(38, 5, dbgPtclStdFmt, p->sinXStart);
        DbgPrintf(38, 6, dbgPtclRateFmt, (p->sinXRate));
        if (u->flags & 0x80) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        } else if (p->sinXLen <= 0) {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
        DbgPrintf(23, 4, dbgPtclSy0Fmt, (p->sy[0]));
        DbgPrintf(31, 5, dbgPtclVal51fFmt, (p->sy[1]));
        DbgPrintf(23, 6, dbgPtclRateStFmt, (p->sy[2]));
        DbgPrintf(31, 7, dbgPtclVal51fFmt, (p->sy[3]));
        DbgPrintf(28, 8, dbgPtclC1fFmt, (p->sy[4]));
        DbgPrintf(31, 9, dbgPtclVal51fFmt, (p->sy[5]));
        DbgPrintf(28, 10, dbgPtclC2fFmt, (p->sy[6]));
        DbgPrintf(31, 11, dbgPtclVal51fFmt, (p->sy[7]));
        DbgPrintf(28, 12, dbgPtclEdfFmt, (p->sy[8]));
        DbgPrintf(31, 13, dbgPtclVal51fFmt, (p->sy[9]));
        DbgPrintf(23, 14, dbgPtclC1dFmt, p->syC1);
        DbgPrintf(23, 15, dbgPtclC2dFmt, p->syC2);
        DbgPrintf(23, 16, dbgPtclStC1Fmt, (u8)p->syCurve[0]);
        DbgPrintf(23, 17, dbgPtclC1C2Fmt, (u8)p->syCurve[1]);
        DbgPrintf(23, 18, dbgPtclC2EdFmt, (u8)p->syCurve[2]);
        DbgPrintf(38, 7, dbgPtclSinYLFmt, p->sinYLen);
        if (p->sinYLen <= 0) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(38, 8, dbgPtclStdFmt, p->sinYStart);
        DbgPrintf(38, 9, dbgPtclRateFmt, (p->sinYRate));
        if ((u->flags & 0x80) || p->sinYLen <= 0) {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
    }
}
