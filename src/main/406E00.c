/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclCtrlGenRotPage(void *cfg, PtclGenDef *o) {
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
            o->rot0[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 2:
            o->rot0[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 3:
            o->rot0[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 4:
            o->rot0[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 5:
            o->rot0[4] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 6:
            o->rot0[5] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 7:
            o->rdxMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 8:
            o->rdx[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 9:
            o->rdx[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 10:
            o->rdx[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 11:
            o->rdx[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 12:
            o->rdyMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 13:
            o->rdy[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 14:
            o->rdy[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 15:
            o->rdy[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 16:
            o->rdy[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 17:
            o->rdzMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 18:
            o->rdz[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 19:
            o->rdz[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 20:
            o->rdz[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 21:
            o->rdz[3] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        }
    }
    if (sysState.dbgCursor < 7) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 4);
    }
    DbgPrintf(3, 3, dbgPtclRx0Fmt, o->rot0[0] & 0xFFFFF, (o->rot0[0] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(8, 4, dbgPtclAngleValFmt, o->rot0[1] & 0xFFFFF, (o->rot0[1] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(3, 5, dbgPtclRy0Fmt, o->rot0[2] & 0xFFFFF, (o->rot0[2] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(8, 6, dbgPtclAngleValFmt, o->rot0[3] & 0xFFFFF, (o->rot0[3] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(3, 7, dbgPtclRz0Fmt, o->rot0[4] & 0xFFFFF, (o->rot0[4] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(8, 8, dbgPtclAngleValFmt, o->rot0[5] & 0xFFFFF, (o->rot0[5] & 0xFFFFF) * (360.0f / 65536.0f));
    if (!o->rdxMode) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(23, 3, dbgPtclRdxMdFmt, o->rdxMode);
    DbgPrintf(23, 4, dbgPtclRdxStartFmt, o->rdx[0] & 0xFFFFF, (o->rdx[0] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(29, 5, dbgPtclAngleValFmt, o->rdx[1] & 0xFFFFF, (o->rdx[1] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(23, 6, dbgPtclRdxEndFmt, o->rdx[2] & 0xFFFFF, (o->rdx[2] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(29, 7, dbgPtclAngleValFmt, o->rdx[3] & 0xFFFFF, (o->rdx[3] & 0xFFFFF) * (360.0f / 65536.0f));
    if (!o->rdyMode) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    } else {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
    DbgPrintf(23, 8, dbgPtclRdyMdFmt, (u8)o->rdyMode);
    DbgPrintf(23, 9, dbgPtclRdyStartFmt, o->rdy[0] & 0xFFFFF, (o->rdy[0] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(29, 10, dbgPtclAngleValFmt, o->rdy[1] & 0xFFFFF, (o->rdy[1] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(23, 11, dbgPtclRdyEndFmt, o->rdy[2] & 0xFFFFF, (o->rdy[2] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(29, 12, dbgPtclAngleValFmt, o->rdy[3] & 0xFFFFF, (o->rdy[3] & 0xFFFFF) * (360.0f / 65536.0f));
    if (!o->rdzMode) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    } else {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
    DbgPrintf(23, 13, dbgPtclRdzMdFmt, (u8)o->rdzMode);
    DbgPrintf(23, 14, dbgPtclRdzStartFmt, o->rdz[0] & 0xFFFFF, (o->rdz[0] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(29, 15, dbgPtclAngleValFmt, o->rdz[1] & 0xFFFFF, (o->rdz[1] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(23, 16, dbgPtclRdzEndFmt, o->rdz[2] & 0xFFFFF, (o->rdz[2] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(29, 17, dbgPtclAngleValFmt, o->rdz[3] & 0xFFFFF, (o->rdz[3] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(0, 0, dbgPtclColor0Str);
}
