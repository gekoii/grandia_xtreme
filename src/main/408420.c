#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"


void DbgPtclGenEmitPage(s32 arg0) {
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
                    sysState.dbgCursor = 24;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 25) {
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
                u->flags ^= 4;
            }
            break;
        case 2:
            if (pad->repeat & 0xA000) {
                u->flags ^= 8;
            }
            break;
        case 3:
            p->emitAngX += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 4:
            p->emitAngXRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 5:
            p->emitAngY += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 6:
            p->emitAngYRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 7:
            p->speed += DbgEditValue(2, -2, 2, 1.0f);
            break;
        case 8:
            p->speedRange += DbgEditValue(2, -2, 2, 1.0f);
            break;
        case 9:
            if (pad->repeat & 0xA000) {
                u->flags ^= 0x10;
            }
            break;
        case 10:
            if (pad->repeat & 0xA000) {
                u->flags ^= 0x20;
            }
            break;
        case 11:
            p->accel += DbgEditValue(2, -2, 2, 0.1f);
            break;
        case 12:
            p->accelRange += DbgEditValue(2, -2, 2, 0.1f);
            break;
        case 13:
            p->life += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 14:
            p->lifeRange += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 15:
            p->rate += DbgEditValue(2, -3, 3, 0.1f);
            break;
        case 16:
            p->rateRange += DbgEditValue(2, -3, 3, 0.1f);
            break;
        case 17:
            p->radius += DbgEditValue(2, -1, 3, 10.0f);
            break;
        case 18:
            p->radiusRange += DbgEditValue(2, -1, 3, 10.0f);
            break;
        case 19:
            p->angX += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 20:
            p->angXRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 21:
            p->angY += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 22:
            p->angYRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 23:
            if (pad->repeat & 0xA000) {
                u->flags ^= 0x200;
            }
            break;
        case 24:
            p->onceNum += DbgEditValue(0, 0, 2, 1.0f);
            break;
        }
    }
    if (sysState.dbgCursor < 17) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 14);
    }
    if (u != NULL) {
        DbgPrintf(3, 3, dbgPtclUseAxFmt, (u->flags & 4) ? dbgPtclOnStr : dbgPtclOffStr);
        DbgPrintf(3, 4, dbgPtclUseAyFmt, (u->flags & 8) ? dbgPtclOnStr : dbgPtclOffStr);
        if (!(u->flags & 4)) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(3, 5, dbgPtclAxFmt, p->emitAngX & 0xFFFFF, (p->emitAngX & 0xFFFFF) * 0.005493164f);
        DbgPrintf(4, 6, dbgPtclAxWidthFmt, p->emitAngXRange & 0xFFFFF, (p->emitAngXRange & 0xFFFFF) * 0.005493164f);
        if (!(u->flags & 8)) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        } else if (!(u->flags & 4)) {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
        DbgPrintf(3, 7, dbgPtclAyFmt, p->emitAngY & 0xFFFFF, (p->emitAngY & 0xFFFFF) * 0.005493164f);
        DbgPrintf(4, 8, dbgPtclAxWidthFmt, p->emitAngYRange & 0xFFFFF, (p->emitAngYRange & 0xFFFFF) * 0.005493164f);
        if (!(u->flags & 8)) {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
        DbgPrintf(3, 9, dbgPtclSpdFmt, p->speed);
        DbgPrintf(8, 10, dbgPtclVal62fFmt, p->speedRange);
        DbgPrintf(3, 11, dbgPtclUseAccFmt, (u->flags & 0x10) ? dbgPtclOnStr : dbgPtclOffStr);
        if (!(u->flags & 0x10)) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(3, 12, dbgPtclAccStopFmt, (u->flags & 0x20) ? dbgPtclOnStr : dbgPtclOffStr);
        if (u->flags & 0x20) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        }
        DbgPrintf(3, 13, dbgPtclAccFmt, p->accel);
        DbgPrintf(8, 14, dbgPtclVal62fFmt, p->accelRange);
        if (!(u->flags & 0x10) || (u->flags & 0x20)) {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
        DbgPrintf(3, 15, dbgPtclLifeFmt, p->life);
        DbgPrintf(8, 16, dbgPtclVal4dFmt, p->lifeRange);
        DbgPrintf(3, 17, dbgPtclProbFmt, p->rate);
        DbgPrintf(8, 18, dbgPtclVal43fFmt, p->rateRange);
        DbgPrintf(23, 3, dbgPtclRadiusFmt, p->radius);
        DbgPrintf(26, 4, dbgPtclVal51fFmt, p->radiusRange);
        DbgPrintf(23, 5, dbgPtclAngXFmt, p->angX & 0xFFFFF, (p->angX & 0xFFFFF) * 0.005493164f);
        DbgPrintf(27, 6, dbgPtclAngXWidthFmt, p->angXRange & 0xFFFFF, (p->angXRange & 0xFFFFF) * 0.005493164f);
        DbgPrintf(27, 7, dbgPtclAngYFmt, p->angY & 0xFFFFF, (p->angY & 0xFFFFF) * 0.005493164f);
        DbgPrintf(27, 8, dbgPtclAngYWidthFmt, p->angYRange & 0xFFFFF, (p->angYRange & 0xFFFFF) * 0.005493164f);
        DbgPrintf(23, 9, dbgPtclAddModeFmt, (u->flags & 0x200) ? dbgPtclOnStr : dbgPtclOffStr);
        DbgPrintf(23, 10, dbgPtclOnceNumFmt, p->onceNum);
    }
}
