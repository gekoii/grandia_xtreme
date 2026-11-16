#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclGenRotPage(s32 arg0) {
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
                    sysState.dbgCursor = 21;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 22) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        if (u == NULL) {
            sysState.dbgCursor = 0;
        }
        switch (sysState.dbgCursor) {
        case 1:
            p->rot0[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 2:
            p->rot0[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 3:
            p->rot0[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 4:
            p->rot0[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 5:
            p->rot0[4] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 6:
            p->rot0[5] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 7:
            p->rdxMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 8:
            p->rdx[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 9:
            p->rdx[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 10:
            p->rdx[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 11:
            p->rdx[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 12:
            p->rdyMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 13:
            p->rdy[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 14:
            p->rdy[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 15:
            p->rdy[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 16:
            p->rdy[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 17:
            p->rdzMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 18:
            p->rdz[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 19:
            p->rdz[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 20:
            p->rdz[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 21:
            p->rdz[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        }
    }
    if (sysState.dbgCursor < 7) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 4);
    }
    if (u != NULL) {
        DbgPrintf(3, 3, dbgPtclRx0Fmt, p->rot0[0] & 0xFFFFF, (p->rot0[0] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(8, 4, dbgPtclAngleValFmt, p->rot0[1] & 0xFFFFF, (p->rot0[1] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(3, 5, dbgPtclRy0Fmt, p->rot0[2] & 0xFFFFF, (p->rot0[2] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(8, 6, dbgPtclAngleValFmt, p->rot0[3] & 0xFFFFF, (p->rot0[3] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(3, 7, dbgPtclRz0Fmt, p->rot0[4] & 0xFFFFF, (p->rot0[4] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(8, 8, dbgPtclAngleValFmt, p->rot0[5] & 0xFFFFF, (p->rot0[5] & 0xFFFFF) * 0.005493164f);
        if (p->rdxMode == 0) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(23, 3, dbgPtclRdxMdFmt, p->rdxMode);
        DbgPrintf(23, 4, dbgPtclRdxStartFmt, p->rdx[0] & 0xFFFFF, (p->rdx[0] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(29, 5, dbgPtclAngleValFmt, p->rdx[1] & 0xFFFFF, (p->rdx[1] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(23, 6, dbgPtclRdxEndFmt, p->rdx[2] & 0xFFFFF, (p->rdx[2] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(29, 7, dbgPtclAngleValFmt, p->rdx[3] & 0xFFFFF, (p->rdx[3] & 0xFFFFF) * 0.005493164f);
        if (p->rdyMode == 0) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        } else {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
        DbgPrintf(23, 8, dbgPtclRdyMdFmt, (u8)p->rdyMode);
        DbgPrintf(23, 9, dbgPtclRdyStartFmt, p->rdy[0] & 0xFFFFF, (p->rdy[0] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(29, 10, dbgPtclAngleValFmt, p->rdy[1] & 0xFFFFF, (p->rdy[1] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(23, 11, dbgPtclRdyEndFmt, p->rdy[2] & 0xFFFFF, (p->rdy[2] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(29, 12, dbgPtclAngleValFmt, p->rdy[3] & 0xFFFFF, (p->rdy[3] & 0xFFFFF) * 0.005493164f);
        if (p->rdzMode == 0) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        } else {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
        DbgPrintf(23, 13, dbgPtclRdzMdFmt, (u8)p->rdzMode);
        DbgPrintf(23, 14, dbgPtclRdzStartFmt, p->rdz[0] & 0xFFFFF, (p->rdz[0] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(29, 15, dbgPtclAngleValFmt, p->rdz[1] & 0xFFFFF, (p->rdz[1] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(23, 16, dbgPtclRdzEndFmt, p->rdz[2] & 0xFFFFF, (p->rdz[2] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(29, 17, dbgPtclAngleValFmt, p->rdz[3] & 0xFFFFF, (p->rdz[3] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
}
