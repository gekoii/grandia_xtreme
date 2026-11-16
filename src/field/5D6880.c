/* rodata: 0x006717F8-0x006718B8 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "gph.h"
#include "field/state.h"
#include "field/n16.h"

extern s32 dbgFldCollisionLine;
extern s32 dbgFldCollisionTextX;
extern s32 dbgFldCollisionTextY;

void DbgFldCollisionPage(void) {
    Pad *pad;
    s32 flags;
    ChrObj *col;
    s32 ang[3];

    pad = &padPort1;
    flags = MapCollGetObjFlags();
    DbgDrawCursor(3, dbgFldCollisionLine + 5);
    if (!(pad->held & 2)) {
        if (pad->repeatFast & 0x1000) {
            if (dbgFldCollisionLine != 0) {
                dbgFldCollisionLine--;
            } else {
                dbgFldCollisionLine = 3;
            }
        }
        if (pad->repeatFast & 0x4000) {
            if (dbgFldCollisionLine < 3) {
                dbgFldCollisionLine++;
            } else {
                dbgFldCollisionLine = 0;
            }
        }
        if (pad->repeatFast & 0x8000) {
            switch (dbgFldCollisionLine) {
            case 0:
                if (flags & 1) {
                    flags ^= 1;
                }
                break;
            case 1:
                flags &= 0xFFFFFFFD;
                break;
            case 2:
                flags &= 0xFFFFFFFB;
                break;
            case 3:
                flags &= 0xFFFFFFF7;
                break;
            }
            MapCollSetFlags(flags);
        }
        if (pad->repeatFast & 0x2000) {
            switch (dbgFldCollisionLine) {
            case 0:
                flags |= 1;
                break;
            case 1:
                flags |= 2;
                break;
            case 2:
                flags |= 4;
                break;
            case 3:
                flags |= 8;
                break;
            }
            MapCollSetFlags(flags);
        }
    }
    DbgPrintf(4, 4, "COLLISION MENU");
    DbgPrintf(4, 5, "CHAR    %s", dbgFldOffOnStr[flags & 1]);
    DbgPrintf(4, 6, "BOARDER %s", dbgFldOffOnStr[(flags >> 1) & 1]);
    DbgPrintf(4, 7, "HTA     %s", dbgFldOffOnStr[(flags >> 2) & 1]);
    DbgPrintf(4, 8, "POLYGON %s : %d", dbgFldOffOnStr[(flags >> 3) & 1], dbgFldPolygonCount);
    col = ChrGetByNo(1);
    DbgPrintf(dbgFldCollisionTextX, dbgFldCollisionTextY + 3, "vector X %.3f", col->vel[0]);
    DbgPrintf(dbgFldCollisionTextX, dbgFldCollisionTextY + 4, "       Y %.3f", col->vel[1]);
    DbgPrintf(dbgFldCollisionTextX, dbgFldCollisionTextY + 5, "       Z %.3f", col->vel[2]);
    ang[0] = col->model->rot[0] * 0.0054931640625f;
    ang[1] = col->model->rot[1] * 0.0054931640625f;
    ang[2] = col->model->rot[2] * 0.0054931640625f;
    DbgPrintf(dbgFldCollisionTextX, dbgFldCollisionTextY + 6, "angle  X %d", ang[0]);
    DbgPrintf(dbgFldCollisionTextX, dbgFldCollisionTextY + 7, "       Y %d", ang[1]);
    DbgPrintf(dbgFldCollisionTextX, dbgFldCollisionTextY + 8, "       Z %d", ang[2]);
    DbgPrintf(dbgFldCollisionTextX, dbgFldCollisionTextY + 9, "distance %.3f", col->leaderDist);
}
