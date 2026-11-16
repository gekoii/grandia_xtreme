/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

void DbgPtclCtrlGenPosPage(s32 arg0, PtclGenDef *o) {
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        if (!(padPort1.held & 0x20)) {
            if (padPort1.repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 14;
                }
            } else if (padPort1.repeat & 0x4000) {
                if (++sysState.dbgCursor >= 15) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        switch (sysState.dbgCursor) {
        case 1:
            o->pos[0] += DbgEditValue(2, -1, 4, 10.0f);
            break;
        case 2:
            o->pos[1] += DbgEditValue(2, -1, 4, 10.0f);
            break;
        case 3:
            o->pos[2] += DbgEditValue(2, -1, 4, 10.0f);
            break;
        case 4:
            o->rot[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 5:
            o->rot[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 6:
            o->rot[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 7:
            o->effectors[0] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 8:
            o->effectors[1] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 9:
            o->effectors[2] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 10:
            o->effectors[3] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 11:
            o->color[0] += DbgEditValue(2, -1, 3, 1.0f);
            o->color[0] = (o->color[0] < 0.0f) ? 0.0f : (255.0f < o->color[0]) ? 255.0f : o->color[0];
            break;
        case 12:
            o->color[1] += DbgEditValue(2, -1, 3, 1.0f);
            o->color[1] = (o->color[1] < 0.0f) ? 0.0f : (255.0f < o->color[1]) ? 255.0f : o->color[1];
            break;
        case 13:
            o->color[2] += DbgEditValue(2, -1, 3, 1.0f);
            o->color[2] = (o->color[2] < 0.0f) ? 0.0f : (255.0f < o->color[2]) ? 255.0f : o->color[2];
            break;
        case 14:
            o->color[3] += DbgEditValue(2, -1, 3, 1.0f);
            o->color[3] = (o->color[3] < 0.0f) ? 0.0f : (255.0f < o->color[3]) ? 255.0f : o->color[3];
            break;
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    DbgPrintf(3, 3, dbgPosXFmt, o->pos[0]);
    DbgPrintf(7, 4, dbgPosYFmt, o->pos[1]);
    DbgPrintf(7, 5, dbgPosZFmt, o->pos[2]);
    DbgPrintf(3, 6, dbgRotXFmt, o->rot[0] & 0xFFFFF, (o->rot[0] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(7, 7, dbgRotYFmt, o->rot[1] & 0xFFFFF, (o->rot[1] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(7, 8, dbgRotZFmt, o->rot[2] & 0xFFFFF, (o->rot[2] & 0xFFFFF) * (360.0f / 65536.0f));
    DbgPrintf(3, 9, dbgPtclEffeAFmt, o->effectors[0]);
    DbgPrintf(8, 10, dbgPtclEffeBFmt, o->effectors[1]);
    DbgPrintf(8, 11, dbgPtclEffeCFmt, o->effectors[2]);
    DbgPrintf(8, 12, dbgPtclEffeDFmt, o->effectors[3]);
    DbgPrintf(3, 13, dbgPtclAmbRFmt, o->color[0]);
    DbgPrintf(7, 14, dbgPtclAmbGFmt, o->color[1]);
    DbgPrintf(7, 15, dbgPtclAmbBFmt, o->color[2]);
    DbgPrintf(7, 16, dbgPtclAmbAFmt, o->color[3]);
}
