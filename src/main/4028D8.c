/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"
#include "ptcl.h"
#include "main/n01.h"

extern u8 dbgPtclStarterPageTitle[];
extern u8 dbgPtclStartMaskFmt[];
extern u8 dbgOfsPosYFmt[];
extern u8 dbgPtclCountFmt[];
extern u8 dbgPtclCtrlFmt[];
extern u8 dbgPtclIdFmt[];
extern u8 dbgPtclWorkFmt[];
extern u8 dbgPtclFileFmt[];
extern u8 dbgPtclSortFmt[];
extern u8 dbgPtclMgrFmt[];
extern u8 dbgPtclInstFmt[];
extern u8 dbgPtclAllFmt[];

void DbgPtclStarterPage(void) {
    Pad *pad;
    PtclStarterEntry *p;
    PtclStarterEntry *q;
    s32 i;
    s32 ok;
    s32 j;
    s32 x;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (pad->trigger & 4) {
            if (--sysState.dbgPage < 0) {
                sysState.dbgPage = 7;
            }
        } else if (pad->trigger & 8) {
            if (++sysState.dbgPage >= 8) {
                sysState.dbgPage = 0;
            }
        }
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 20;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 21) {
                    sysState.dbgCursor = 0;
                }
            }
        }
        if (sysState.dbgCursor == 0) {
            if (pad->repeat & 0x20) {
                ok = 1;
                for (j = 0; j < 16; j++) {
                    if (PtclStarterKill(j) == 0) {
                        ok = 0;
                    }
                }
                if (ptclStarterFrame < 0) {
                    if (ok) {
                        ptclStarterFrame = 0;
                        ptclPeakWork = 0;
                        ptclCurWork = 0;
                    }
                } else {
                    ptclStarterFrame = -1;
                }
            }
        } else if (sysState.dbgCursor == 1) {
            ptclStartMaskId += DbgEditValue(0, 0, 1, 1.0f);
        } else if (sysState.dbgCursor == 2) {
            ptclStartOffsetY += DbgEditValue(2, -1, 4, 10.0f);
        } else {
            q = ptclStarterTable;
            q += (sysState.dbgCursor - 3) / 9 + sysState.dbgPage * 2;
            switch ((sysState.dbgCursor - 3) % 9) {
            case 0:
                q->time += DbgEditValue(0, 0, 3, 1.0f);
                break;
            case 1:
                q->ctrl += DbgEditValue(0, 0, 1, 1.0f);
                break;
            case 2:
                q->id += DbgEditValue(0, 0, 1, 1.0f);
                break;
            case 3:
                q->pos[0] += DbgEditValue(2, -1, 4, 10.0f);
                break;
            case 4:
                q->pos[1] += DbgEditValue(2, -1, 4, 10.0f);
                break;
            case 5:
                q->pos[2] += DbgEditValue(2, -1, 4, 10.0f);
                break;
            case 6:
                q->rot[0] += DbgEditValue(1, 0, 5, 256.0f);
                break;
            case 7:
                q->rot[1] += DbgEditValue(1, 0, 5, 256.0f);
                break;
            case 8:
                q->rot[2] += DbgEditValue(1, 0, 5, 256.0f);
                break;
            }
        }
    }
    DbgPrintf(3, 2, dbgPtclStarterPageTitle, dbgPtclTitleColorStr, sysState.dbgPage + 1, 8, dbgPtclColor0Str);
    DbgPrintf(3, 3, dbgPtclStartMaskFmt, ptclStartMaskId);
    DbgPrintf(3, 4, dbgOfsPosYFmt, ptclStartOffsetY);
    if (sysState.dbgCursor < 12) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 7);
    }
    p = &ptclStarterTable[sysState.dbgPage * 2];
    for (i = 0; i < 2; i++, p++) {
        x = (i == 0) ? 3 : 23;
        if (p->time < 0) {
            DbgPrintf(0, 0, dbgPtclColor4Str);
        } else {
            DbgPrintf(0, 0, dbgPtclColor0Str);
        }
        DbgPrintf(x, 5, dbgPtclCountFmt, p->time);
        DbgPrintf(x, 6, dbgPtclCtrlFmt, p->ctrl);
        DbgPrintf(x, 7, dbgPtclIdFmt, p->id);
        DbgPrintf(x, 8, dbgPosXFmt, p->pos[0]);
        DbgPrintf(x + 4, 9, dbgPosYFmt, p->pos[1]);
        DbgPrintf(x + 4, 10, dbgPosZFmt, p->pos[2]);
        DbgPrintf(x, 11, dbgRotXFmt, p->rot[0] & 0xFFFFF, (p->rot[0] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(x + 4, 12, dbgRotYFmt, p->rot[1] & 0xFFFFF, (p->rot[1] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(x + 4, 13, dbgRotZFmt, p->rot[2] & 0xFFFFF, (p->rot[2] & 0xFFFFF) * 0.005493164f);
    }
    DbgPrintf(0, 0, dbgPtclColor0Str);
    DbgPrintf(3, 15, dbgPtclWorkFmt, ptclPeakWork, ptclCurWork);
    DbgPrintf(3, 16, dbgPtclFileFmt, ptclFileSize);
    DbgPrintf(3, 17, dbgPtclSortFmt, ptclSortAllocSize);
    DbgPrintf(3, 18, dbgPtclMgrFmt, GphTexTableSize(0x20) + PtclSystemCalcSize(0x20, 8, 0x20));
    DbgPrintf(23, 16, dbgPtclFileFmt, ptclDbgExtFileSize);
    DbgPrintf(23, 17, dbgPtclSortFmt, ptclDbgExtSortSize);
    DbgPrintf(23, 18, dbgPtclInstFmt, ptclDbgExtInstSize);
    DbgPrintf(23, 19, dbgPtclAllFmt, ptclPeakWork + ptclFileSize + ptclSortAllocSize + (u32)GphTexTableSize(0x20) + PtclSystemCalcSize(0x20, 8, 0x20) + ptclDbgExtFileSize + ptclDbgExtSortSize + ptclDbgExtInstSize);
}
