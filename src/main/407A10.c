/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

extern s32 dbgPtclTexIdFmt;

void DbgPtclGenPosPage(s32 arg0) {
    PtclGen *u;
    PtclGenDef *p;
    Pad *pad;
    s32 v;
    s32 r;

    u = ptclMainPool->gens[arg0];
    p = (u != NULL) ? &u->def : NULL;
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
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
        if (u == NULL) {
            sysState.dbgCursor = 0;
        }
        switch (sysState.dbgCursor) {
        case 1:
            p->pos[0] += DbgEditValue(2, -1, 4, 10.0f);
            break;
        case 2:
            p->pos[1] += DbgEditValue(2, -1, 4, 10.0f);
            break;
        case 3:
            p->pos[2] += DbgEditValue(2, -1, 4, 10.0f);
            break;
        case 4:
            p->rot[0] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 5:
            p->rot[1] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 6:
            p->rot[2] += DbgEditValue(1, 0, 5, 256.0f);
            break;
        case 7:
            p->effectors[0] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 8:
            p->effectors[1] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 9:
            p->effectors[2] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 10:
            p->effectors[3] += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 11:
            if (pad->repeat & 0xA000) {
                u->flags ^= 1;
            }
            break;
        case 12:
            if (pad->repeat & 0xA000) {
                u->flags ^= 2;
            }
            break;
        case 13:
            p->color[0] += DbgEditValue(2, -1, 3, 1.0f);
            p->color[0] = (p->color[0] < 0.0f) ? 0.0f : (p->color[0] > 255.0f) ? 255.0f : p->color[0];
            break;
        case 14:
            p->color[1] += DbgEditValue(2, -1, 3, 1.0f);
            p->color[1] = (p->color[1] < 0.0f) ? 0.0f : (p->color[1] > 255.0f) ? 255.0f : p->color[1];
            break;
        case 15:
            p->color[2] += DbgEditValue(2, -1, 3, 1.0f);
            p->color[2] = (p->color[2] < 0.0f) ? 0.0f : (p->color[2] > 255.0f) ? 255.0f : p->color[2];
            break;
        case 16:
            p->color[3] += DbgEditValue(2, -1, 3, 1.0f);
            p->color[3] = (p->color[3] < 0.0f) ? 0.0f : (p->color[3] > 255.0f) ? 255.0f : p->color[3];
            break;
        case 17:
            u->blendMode += DbgEditValue(0, 0, 1, 1.0f);
            break;
        case 18:
            if (u->flags & 0x1000) {
                v = 1;
            } else {
                v = (u->flags & 0x2000) ? 2 : 0;
            }
            v += DbgEditValue(0, 0, 1, 1.0f);
            if (v >= 0) {
                r = v;
                if (r > 2) {
                    r = 2;
                }
            } else {
                r = 0;
            }
            u->flags &= 0xFFFFCFFF;
            if (r == 1) {
                u->flags |= 0x1000;
            } else if (r == 2) {
                u->flags |= 0x2000;
            }
            break;
        case 19:
            if (u->flags & 0x4000) {
                v = 1;
            } else {
                v = (u->flags & 0x8000) ? 2 : 0;
            }
            v += DbgEditValue(0, 0, 1, 1.0f);
            if (v >= 0) {
                r = v;
                if (r > 2) {
                    r = 2;
                }
            } else {
                r = 0;
            }
            u->flags &= 0xFFFF3FFF;
            if (r == 1) {
                u->flags |= 0x4000;
            } else if (r == 2) {
                u->flags |= 0x8000;
            }
            break;
        case 20:
            if (pad->repeat & 0xA000) {
                u->flags ^= 0x800;
            }
            break;
        }
    }
    if (sysState.dbgCursor < 18) {
        DbgDrawCursor(2, sysState.dbgCursor + 2);
    } else {
        DbgDrawCursor(22, sysState.dbgCursor - 15);
    }
    if (u != NULL) {
        DbgPrintf(3, 3, dbgPosXFmt, p->pos[0]);
        DbgPrintf(7, 4, dbgPosYFmt, p->pos[1]);
        DbgPrintf(7, 5, dbgPosZFmt, p->pos[2]);
        DbgPrintf(3, 6, dbgRotXFmt, p->rot[0] & 0xFFFFF, (p->rot[0] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(7, 7, dbgRotYFmt, p->rot[1] & 0xFFFFF, (p->rot[1] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(7, 8, dbgRotZFmt, p->rot[2] & 0xFFFFF, (p->rot[2] & 0xFFFFF) * 0.005493164f);
        DbgPrintf(3, 9, dbgPtclEffeAFmt, p->effectors[0]);
        DbgPrintf(8, 10, dbgPtclEffeBFmt, p->effectors[1]);
        DbgPrintf(8, 11, dbgPtclEffeCFmt, p->effectors[2]);
        DbgPrintf(8, 12, dbgPtclEffeDFmt, p->effectors[3]);
        DbgPrintf(3, 13, dbgPtclDirGeneFmt, (u->flags & 1) ? dbgPtclOnStr : dbgPtclOffStr);
        DbgPrintf(3, 14, dbgPtclFollowFmt, (u->flags & 2) ? dbgPtclOnStr : dbgPtclOffStr);
        DbgPrintf(3, 15, dbgPtclAmbRFmt, p->color[0]);
        DbgPrintf(7, 16, dbgPtclAmbGFmt, p->color[1]);
        DbgPrintf(7, 17, dbgPtclAmbBFmt, p->color[2]);
        DbgPrintf(7, 18, dbgPtclAmbAFmt, p->color[3]);
        DbgPrintf(3, 19, dbgPtclAlphaFmt, u->blendMode);
        DbgPrintf(23, 3, dbgPtclRhFmt, (u->flags & 0x1000) ? dbgPtclOnStr : (u->flags & 0x2000) ? dbgPtclRndStr : dbgPtclOffStr);
        DbgPrintf(23, 4, dbgPtclRvFmt, (u->flags & 0x4000) ? dbgPtclOnStr : (u->flags & 0x8000) ? dbgPtclRndStr : dbgPtclOffStr);
        DbgPrintf(23, 5, dbgPtclRXyzFmt, (u->flags & 0x800) ? dbgPtclOnStr : dbgPtclOffStr);
        DbgPrintf(23, 6, &dbgPtclTexIdFmt, u->texId);
    }
}
