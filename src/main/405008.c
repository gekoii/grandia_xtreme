/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclCtrlGenEmitPage(PtclCtrl *cfg, PtclGenDef *o) {
    Pad *pad;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 19;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 20) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        switch (sysState.dbgCursor) {
        case 1:
            o->emitAngX += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 2:
            o->emitAngXRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 3:
            o->emitAngY += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 4:
            o->emitAngYRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 5:
            o->speed += DbgEditValue(2, -2, 2, 1.0f);
            break;
        case 6:
            o->speedRange += DbgEditValue(2, -2, 2, 1.0f);
            break;
        case 7:
            o->accel += DbgEditValue(2, -2, 2, 0.1f);
            break;
        case 8:
            o->accelRange += DbgEditValue(2, -2, 2, 0.1f);
            break;
        case 9:
            o->life += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 10:
            o->lifeRange += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 11:
            o->rate += DbgEditValue(2, -3, 3, 0.1f);
            break;
        case 12:
            o->rateRange += DbgEditValue(2, -3, 3, 0.1f);
            break;
        case 13:
            o->radius += DbgEditValue(2, -1, 3, 10.0f);
            break;
        case 14:
            o->radiusRange += DbgEditValue(2, -1, 3, 10.0f);
            break;
        case 15:
            o->angX += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 16:
            o->angXRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 17:
            o->angY += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 18:
            o->angYRange += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 19:
            o->onceNum += DbgEditValue(0, 0, 2, 1.0f);
            break;
        }
    }
    if (sysState.dbgCursor < 13) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 10);
    }
    if (!(cfg->genFlags & 4)) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(3, 3, dbgPtclAxFmt, o->emitAngX & 0xFFFFF, (o->emitAngX & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(4, 4, dbgPtclAxWidthFmt, o->emitAngXRange & 0xFFFFF, (o->emitAngXRange & 0xFFFFF) * (360.0f / 65536.0f));
    if (!(cfg->genFlags & 8)) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    } else if (!(cfg->genFlags & 4)) {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
    DbgPrintf(3, 5, dbgPtclAyFmt, o->emitAngY & 0xFFFFF, (o->emitAngY & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(4, 6, dbgPtclAxWidthFmt, o->emitAngYRange & 0xFFFFF, (o->emitAngYRange & 0xFFFFF) * (360.0f / 65536.0f));
    if (!(cfg->genFlags & 8)) {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
    DbgPrintf(3, 7, dbgPtclSpdFmt, o->speed);
    DbgPrintf(8, 8, dbgPtclVal62fFmt, o->speedRange);
    if (!(cfg->genFlags & 0x10)) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    if (cfg->genFlags & 0x20) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(3, 9, dbgPtclAccFmt, o->accel);
    DbgPrintf(8, 10, dbgPtclVal62fFmt, o->accelRange);
    /* s64 view: the original tests this with an ld, not an lw */
    if ((*(s64 *)&cfg->genFlags & 0x30) != 0x10) {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
    DbgPrintf(3, 11, dbgPtclLifeFmt, o->life);
    DbgPrintf(8, 12, dbgPtclVal4dFmt, o->lifeRange);
    DbgPrintf(3, 13, dbgPtclProbFmt, o->rate);
    DbgPrintf(8, 14, dbgPtclVal43fFmt, o->rateRange);
    DbgPrintf(23, 3, dbgPtclRadiusFmt, o->radius);
    DbgPrintf(26, 4, dbgPtclVal51fFmt, o->radiusRange);
    DbgPrintf(23, 5, dbgPtclAngXFmt, o->angX & 0xFFFFF, (o->angX & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(27, 6, dbgPtclAngXWidthFmt, o->angXRange & 0xFFFFF, (o->angXRange & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(27, 7, dbgPtclAngYFmt, o->angY & 0xFFFFF, (o->angY & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(27, 8, dbgPtclAngYWidthFmt, o->angYRange & 0xFFFFF, (o->angYRange & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(23, 9, dbgPtclOnceNumFmt, o->onceNum);
}
