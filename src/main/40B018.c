/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

extern s32 dbgPtclEffectorIndex;
extern u8 dbgPtclEffectorTitleFmt[];
extern u8 dbgPtclEffIdxFmt[];
extern u8 dbgPtclEffScaleFmt[];
extern u8 dbgPtclEffModeFmt[];
extern u8 dbgPtclEffVectorStr[];
extern u8 dbgPtclEffPosStr[];
extern u8 dbgPtclEffOfsFmt[];
extern u8 dbgPtclVal72fFmt[];
extern u8 dbgPtclEffCycleFmt[];
extern u8 dbgPtclEffSysFmt[];
extern u8 dbgPtclLocalStr[];
extern u8 dbgPtclWorldStr[];

void DbgPtclEffectorPage(void) {
    PtclEffector *e;
    Pad *pad;

    e = &ptclMainPool->effectors[dbgPtclEffectorIndex];
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(padPort1.held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 11;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 12) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        switch (sysState.dbgCursor) {
        case 0:
            dbgPtclEffectorIndex = dbgPtclEffectorIndex + DbgEditValue(0, 0, 3, 1.0f);
            dbgPtclEffectorIndex = (dbgPtclEffectorIndex < 0) ? ptclMainPool->effectorCount - 1
                       : ((ptclMainPool->effectorCount - 1 < dbgPtclEffectorIndex) ? 0 : dbgPtclEffectorIndex);
            e = &ptclMainPool->effectors[dbgPtclEffectorIndex];
            break;
        case 1:
            e->id = e->id + DbgEditValue(0, 0, 4, 1.0f);
            break;
        case 2:
            e->scale += DbgEditValue(2, -2, 4, 0.1f);
            break;
        case 3:
            e->scaleRange += DbgEditValue(2, -2, 4, 0.1f);
            break;
        case 4:
            e->angX = e->angX + DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 5:
            e->angXRange = e->angXRange + DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 6:
            e->angY = e->angY + DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 7:
            e->angYRange = e->angYRange + DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 8:
            if (pad->repeat & 0xA000) {
                e->flags ^= 1;
            }
            break;
        case 9:
            if (pad->repeat & 0xA000) {
                e->flags ^= 2;
            }
            break;
        case 10:
            e->cycle = e->cycle + DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 11:
            if (pad->repeat & 0xA000) {
                e->flags ^= 4;
            }
            break;
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    DbgPrintf(3, 2, dbgPtclEffectorTitleFmt, dbgPtclEffectorIndex);
    DbgPrintf(3, 3, dbgPtclEffIdxFmt, e->id);
    DbgPrintf(3, 4, dbgPtclEffScaleFmt, e->scale);
    DbgPrintf(3, 6, dbgPtclAngXFmt, e->angX & 0xFFFFF, (e->angX & 0xFFFFF) * 0.005493164f);
    DbgPrintf(7, 8, dbgPtclAngYFmt, e->angY & 0xFFFFF, (e->angY & 0xFFFFF) * 0.005493164f);
    DbgPrintf(3, 10, dbgPtclEffModeFmt, (e->flags & 1) ? dbgPtclEffVectorStr : dbgPtclEffPosStr);
    DbgPrintf(3, 11, dbgPtclEffOfsFmt, (e->flags & 2) ? dbgPtclOnStr : dbgPtclOffStr);
    if (!(e->flags & 2)) {
        DbgPrintf(0, 0, dbgPtclColor4Str);
    }
    DbgPrintf(6, 5, dbgPtclVal72fFmt, e->scaleRange);
    DbgPrintf(7, 7, dbgPtclAngXWidthFmt, e->angXRange & 0xFFFFF, (e->angXRange & 0xFFFFF) * 0.005493164f);
    DbgPrintf(7, 9, dbgPtclAngYWidthFmt, e->angYRange & 0xFFFFF, (e->angYRange & 0xFFFFF) * 0.005493164f);
    DbgPrintf(3, 12, dbgPtclEffCycleFmt, e->cycle);
    if (!(e->flags & 2)) {
        DbgPrintf(0, 0, dbgPtclColor0Str);
    }
    DbgPrintf(3, 13, dbgPtclEffSysFmt, (e->flags & 4) ? dbgPtclLocalStr : dbgPtclWorldStr);
}
