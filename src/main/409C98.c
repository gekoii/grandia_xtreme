#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclGenColorPage(s32 arg0) {
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
                    sysState.dbgCursor = 22;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 23) {
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
                u->flags ^= 0x100;
            }
            break;
        case 2:
            p->colorC1 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 3:
            p->colorC2 += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 4:
            p->colorCurve[0] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 5:
            p->colorCurve[1] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 6:
            p->colorCurve[2] += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 7:
            p->colorKeys[0] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 8:
            p->colorKeys[1] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 9:
            p->colorKeys[2] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 10:
            p->colorKeys[3] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 11:
            p->colorKeys[4] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 12:
            p->colorKeys[5] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 13:
            p->colorKeys[6] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 14:
            p->colorKeys[7] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 15:
            p->colorKeys[8] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 16:
            p->colorKeys[9] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 17:
            p->colorKeys[10] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 18:
            p->colorKeys[11] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 19:
            p->colorKeys[12] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 20:
            p->colorKeys[13] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 21:
            p->colorKeys[14] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 22:
            p->colorKeys[15] += DbgEditValue(0, 0, 3, 1.0f);
            break;
        }
    }
    if (sysState.dbgCursor < 7) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 4);
    }
    if (u != NULL) {
        if (u->colorPkt == NULL) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(3, 3, dbgPtclRgbChangeFmt, (u->flags & 0x100) ? dbgPtclOnStr : dbgPtclOffStr);
        DbgPrintf(3, 4, dbgPtclC1dFmt, p->colorC1);
        DbgPrintf(3, 5, dbgPtclC2dFmt, p->colorC2);
        DbgPrintf(3, 6, dbgPtclStC1Fmt, p->colorCurve[0]);
        DbgPrintf(3, 7, dbgPtclC1C2Fmt, p->colorCurve[1]);
        DbgPrintf(3, 8, dbgPtclC2EdFmt, p->colorCurve[2]);
        if (!(u->flags & 0x100)) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(23, 3, dbgPtclStRFmt, p->colorKeys[0] + 0x80);
        DbgPrintf(26, 4, dbgPtclColGFmt, p->colorKeys[1] + 0x80);
        DbgPrintf(26, 5, dbgPtclColBFmt, p->colorKeys[2] + 0x80);
        DbgPrintf(26, 6, dbgPtclColAFmt, p->colorKeys[3] + 0x80);
        DbgPrintf(23, 7, dbgPtclC1RFmt, p->colorKeys[4] + 0x80);
        DbgPrintf(26, 8, dbgPtclColGFmt, p->colorKeys[5] + 0x80);
        DbgPrintf(26, 9, dbgPtclColBFmt, p->colorKeys[6] + 0x80);
        DbgPrintf(26, 10, dbgPtclColAFmt, p->colorKeys[7] + 0x80);
        DbgPrintf(23, 11, dbgPtclC2RFmt, p->colorKeys[8] + 0x80);
        DbgPrintf(26, 12, dbgPtclColGFmt, p->colorKeys[9] + 0x80);
        DbgPrintf(26, 13, dbgPtclColBFmt, p->colorKeys[10] + 0x80);
        DbgPrintf(26, 14, dbgPtclColAFmt, p->colorKeys[11] + 0x80);
        DbgPrintf(23, 15, dbgPtclEdRFmt, p->colorKeys[12] + 0x80);
        DbgPrintf(26, 16, dbgPtclColGFmt, p->colorKeys[13] + 0x80);
        DbgPrintf(26, 17, dbgPtclColBFmt, p->colorKeys[14] + 0x80);
        DbgPrintf(26, 18, dbgPtclColAFmt, p->colorKeys[15] + 0x80);
        if (u->colorPkt == NULL || !(u->flags & 0x100)) {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
    }
}
