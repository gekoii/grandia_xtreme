/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

extern u8 dbgPtclBlockFmt[];
extern u8 dbgPtclUseVcFmt[];
extern u8 dbgPtclGeneParStr[];

/* The ALPHA line edits and prints blendMode (+0xC) as an s32 (lw/sw). */
void DbgPtclCtrlFlagsPage(PtclCtrl *o) {
    Pad *pad;
    s32 v;
    s32 w;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (!(pad->held & 0x20)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 16;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 17) {
                    sysState.dbgCursor = 0;
                }
            }
            if (o == NULL) {
                sysState.dbgCursor = 0;
            }
        }
        switch (sysState.dbgCursor) {
        case 1:
            o->groupCount += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 2:
            if (pad->repeat & 0xA000) {
                o->flags ^= 1;
            }
            break;
        case 3:
            *(s32 *)&o->blendMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 4:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 1;
            }
            break;
        case 5:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 2;
            }
            break;
        case 6:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 4;
            }
            break;
        case 7:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 8;
            }
            break;
        case 8:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 0x10;
            }
            break;
        case 9:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 0x20;
            }
            break;
        case 10:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 0x200;
            }
            break;
        case 11:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 0x80;
            }
            break;
        case 12:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 0x100;
            }
            break;
        case 13:
            if (o->genFlags & 0x1000) {
                v = 1;
            } else if (o->genFlags & 0x2000) {
                v = 2;
            } else {
                v = 0;
            }
            v += DbgEditValue(0, 0, 1, 1.0f);
            w = (v < 0) ? 0 : (v >= 3) ? 2 : v;
            o->genFlags &= 0xFFFFCFFF;
            if (w == 1) {
                o->genFlags |= 0x1000;
            } else if (w == 2) {
                o->genFlags |= 0x2000;
            }
            break;
        case 14:
            if (o->genFlags & 0x4000) {
                v = 1;
            } else if (o->genFlags & 0x8000) {
                v = 2;
            } else {
                v = 0;
            }
            v += DbgEditValue(0, 0, 1, 1.0f);
            w = (v < 0) ? 0 : (v >= 3) ? 2 : v;
            o->genFlags &= 0xFFFF3FFF;
            if (w == 1) {
                o->genFlags |= 0x4000;
            } else if (w == 2) {
                o->genFlags |= 0x8000;
            }
            break;
        case 15:
            if (pad->repeat & 0xA000) {
                o->genFlags ^= 0x800;
            }
            break;
        case 16:
            if (pad->trigger & 0x20) {
                sysState.dbgMenuFlags |= 0x1000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
            }
            break;
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    if (o == NULL) {
        return;
    }
    DbgPrintf(3, 3, dbgPtclBlockFmt, o->groupCount);
    DbgPrintf(3, 4, dbgPtclUseVcFmt, (o->flags & 1) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 5, dbgPtclAlphaFmt, *(s32 *)&o->blendMode);
    DbgPrintf(3, 6, dbgPtclDirGeneFmt, (o->genFlags & 1) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 7, dbgPtclFollowFmt, (o->genFlags & 2) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 8, dbgPtclUseAxFmt, (o->genFlags & 4) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 9, dbgPtclUseAyFmt, (o->genFlags & 8) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 10, dbgPtclUseAccFmt, (o->genFlags & 0x10) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 11, dbgPtclAccStopFmt, (o->genFlags & 0x20) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 12, dbgPtclAddModeFmt, (o->genFlags & 0x200) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 13, dbgPtclSizeLinkFmt, (o->genFlags & 0x80) ? dbgPtclEqualStr : dbgPtclNotEqualStr);
    DbgPrintf(3, 14, dbgPtclRgbChangeFmt, (o->genFlags & 0x100) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 15, dbgPtclRhFmt, (o->genFlags & 0x1000) ? dbgPtclOnStr : (o->genFlags & 0x2000) ? dbgPtclRndStr : dbgPtclOffStr);
    DbgPrintf(3, 16, dbgPtclRvFmt, (o->genFlags & 0x4000) ? dbgPtclOnStr : (o->genFlags & 0x8000) ? dbgPtclRndStr : dbgPtclOffStr);
    DbgPrintf(3, 17, dbgPtclRXyzFmt, (o->genFlags & 0x800) ? dbgPtclOnStr : dbgPtclOffStr);
    DbgPrintf(3, 18, dbgPtclGeneParStr);
}
