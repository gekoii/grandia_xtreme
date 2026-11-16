/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "gph.h"
#include "main/n02.h"

/* GA object as the OBJECT page shows it. gph.h's GphGaObject has no field
 * for +0x8 (printed as the texture id) and types +0x24 and +0x50 as
 * pointers, which this page edits and prints as numbers. */
typedef struct {
    u8 pad0;
    u8 x1;
    s8 x2;
    u8 pad3[5];
    s32 x8;
    u8 padC[4];
    f32 x10;
    f32 x14;
    f32 x18;
    u8 pad1C[4];
    f32 x20;
    s32 x24;
    u8 pad28[0x1C];
    s32 x44;
    s32 x48;
    u8 pad4C[4];
    s32 x50;
} Sub;

extern s32 dbgObjLine;
extern s32 dbgObjCursor;
extern s32 dbgObjObjNo;
extern s32 dbgObjPartsNo;
extern s32 dbgObjGaObjNo;
extern char dbgObjStrObjectMenu[];
extern char dbgObjStrObjNoPartsGaobjFlgId[];
extern char dbgObjStrObjNo[];
extern char dbgObjStrPos[];
extern char dbgObjStrClp[];
extern char dbgObjStrAng[];
extern char dbgObjStrScl[];
extern char dbgObjStrPartsNoFlagId[];
extern char dbgObjStrMax[];
extern char dbgObjStrPartsNo[];
extern char dbgObjStrGaObjNo[];
extern char dbgObjStrAlpha[];
extern char dbgObjStrVertex[];
extern char dbgObjStrPolygon[];
extern char dbgObjStrClipStat[];
extern char dbgObjStrCenter[];
extern char dbgObjStrRangeTexId[];


void DbgObjectPage(void) {
    GphModel *obj;
    GphPart *part;
    Sub *sub;
    s32 v;
    s32 i;
    s32 sum;

    sub = NULL;
    if (dbgObjList == NULL) {
        return;
    }
    dbgPad = &padPort1;
    obj = dbgObjList->objs[dbgObjObjNo];
    part = NULL;
    if (obj != NULL) {
        part = ChrModelGetPart(obj, dbgObjPartsNo);
    }
    if (part != NULL) {
        sub = (Sub *)ChrPartGetGaObj(part, dbgObjGaObjNo);
    }
    if (sub != NULL) {
        sub->x1 = 0;
    }
    if (!(dbgPad->held & 2)) {
        switch (dbgObjLine = DbgMenuLineSelect(dbgObjLine, 9)) {
        case 0:
            dbgObjCursor = 0;
            if (dbgPad->repeatFast & 0x8000) {
                dbgObjObjNo--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgObjObjNo++;
            }
            if (dbgObjObjNo < 0) {
                dbgObjObjNo = 0;
            }
            break;
        case 1:
            dbgObjCursor = 1;
            if (obj == NULL) {
                break;
            }
            v = dbgPad->leftX - 0x80;
            if (v > 60) {
                obj->pos[0] -= 10.0f;
            }
            if (v < -60) {
                obj->pos[0] += 10.0f;
            }
            v = dbgPad->leftY - 0x80;
            if (v > 60) {
                obj->pos[1] += 10.0f;
            }
            if (v < -60) {
                obj->pos[1] -= 10.0f;
            }
            v = dbgPad->rightY - 0x80;
            if (v > 60) {
                obj->pos[2] += 10.0f;
            }
            if (v < -60) {
                obj->pos[2] -= 10.0f;
            }
            break;
        case 2:
            dbgObjCursor = 2;
            if (obj == NULL) {
                break;
            }
            v = dbgPad->leftX - 0x80;
            if (v > 60) {
                obj->cullSphere[0] -= 10.0f;
            }
            if (v < -60) {
                obj->cullSphere[0] += 10.0f;
            }
            v = dbgPad->leftY - 0x80;
            if (v > 60) {
                obj->cullSphere[1] += 10.0f;
            }
            if (v < -60) {
                obj->cullSphere[1] -= 10.0f;
            }
            v = dbgPad->rightY - 0x80;
            if (v > 60) {
                obj->cullSphere[2] += 10.0f;
            }
            if (v < -60) {
                obj->cullSphere[2] -= 10.0f;
            }
            break;
        case 3:
            dbgObjCursor = 3;
            if (obj == NULL) {
                break;
            }
            v = dbgPad->leftX - 0x80;
            if (v > 60) {
                obj->rot[0] -= 0x100;
            }
            if (v < -60) {
                obj->rot[0] += 0x100;
            }
            v = dbgPad->leftY - 0x80;
            if (v > 60) {
                obj->rot[1] += 0x10;
            }
            if (v < -60) {
                obj->rot[1] -= 0x10;
            }
            v = dbgPad->rightY - 0x80;
            if (v > 60) {
                obj->rot[2] += 0x10;
            }
            if (v < -60) {
                obj->rot[2] -= 0x10;
            }
            break;
        case 4:
            dbgObjCursor = 4;
            if (obj == NULL) {
                break;
            }
            v = dbgPad->leftX - 0x80;
            if (v > 60) {
                obj->scale[0] -= 0.01f;
            }
            if (v < -60) {
                obj->scale[0] += 0.01f;
            }
            v = dbgPad->leftY - 0x80;
            if (v > 60) {
                obj->scale[1] += 0.01f;
            }
            if (v < -60) {
                obj->scale[1] -= 0.01f;
            }
            v = dbgPad->rightY - 0x80;
            if (v > 60) {
                obj->scale[2] += 0.01f;
            }
            if (v < -60) {
                obj->scale[2] -= 0.01f;
            }
            break;
        case 5:
            dbgObjCursor = 5;
            if (dbgPad->repeatFast & 0x8000) {
                dbgObjPartsNo--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgObjPartsNo++;
            }
            if (dbgObjPartsNo < 0) {
                dbgObjPartsNo = 0;
            }
            break;
        case 6:
            dbgObjCursor = 6;
            if (part == NULL) {
                break;
            }
            v = dbgPad->leftX - 0x80;
            if (v > 60) {
                part->pos[0] -= 10.0f;
            }
            if (v < -60) {
                part->pos[0] += 10.0f;
            }
            v = dbgPad->leftY - 0x80;
            if (v > 60) {
                part->pos[1] += 10.0f;
            }
            if (v < -60) {
                part->pos[1] -= 10.0f;
            }
            v = dbgPad->rightY - 0x80;
            if (v > 60) {
                part->pos[2] += 10.0f;
            }
            if (v < -60) {
                part->pos[2] -= 10.0f;
            }
            break;
        case 7:
            dbgObjCursor = 7;
            break;
        case 8:
            dbgObjCursor = 8;
            if (obj == NULL) {
                break;
            }
            v = dbgPad->leftX - 0x80;
            if (v > 60) {
                part->rot[0] -= 0x10;
            }
            if (v < -60) {
                part->rot[0] += 0x10;
            }
            v = dbgPad->leftY - 0x80;
            if (v > 60) {
                part->rot[1] += 0x10;
            }
            if (v < -60) {
                part->rot[1] -= 0x10;
            }
            v = dbgPad->rightY - 0x80;
            if (v > 60) {
                part->rot[2] += 0x10;
            }
            if (v < -60) {
                part->rot[2] -= 0x10;
            }
            break;
        case 9:
            dbgObjCursor = 10;
            if (dbgPad->repeatFast & 0x8000) {
                dbgObjGaObjNo--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgObjGaObjNo++;
            }
            if (dbgObjGaObjNo < 0) {
                dbgObjGaObjNo = 0;
            }
            break;
        case 10:
            dbgObjCursor = 10;
            if (sub == NULL) {
                break;
            }
            if (dbgPad->repeatFast & 0x8000) {
                sub->x24--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                sub->x24++;
            }
            break;
        }
    }
    obj = dbgObjList->objs[dbgObjObjNo];
    part = NULL;
    sub = NULL;
    if (obj != NULL) {
        part = ChrModelGetPart(obj, dbgObjPartsNo);
    }
    if (part != NULL) {
        sub = (Sub *)ChrPartGetGaObj(part, dbgObjGaObjNo);
    }
    if (sub != NULL) {
        sub->x1 |= 1;
    }
    DbgMenuCursorDraw(2, dbgObjCursor + 4);
    DbgPuts(2, 3, dbgObjStrObjectMenu);
    if (obj != NULL) {
        sum = 0;
        for (i = 0; i < obj->partCount; i++) {
            part = ChrModelGetPart(obj, i);
            if (part != NULL) {
                sum += part->gaObjCount;
            }
        }
        DbgPrintf(3, 4, dbgObjStrObjNoPartsGaobjFlgId, dbgObjObjNo, obj->partCount, sum, obj->flags, obj->objId);
    } else {
        DbgPrintf(3, 4, dbgObjStrObjNo, dbgObjObjNo);
    }
    if (obj != NULL) {
        DbgPrintf(3, 5, dbgObjStrPos, obj->pos[0], obj->pos[1], obj->pos[2]);
        DbgPrintf(3, 6, dbgObjStrClp, obj->cullSphere[0], obj->cullSphere[1], obj->cullSphere[2], obj->cullSphere[3]);
        DbgPrintf(3, 7, dbgObjStrAng, obj->rot[0] * 0.0054931640625f, obj->rot[1] * 0.0054931640625f,
                      obj->rot[2] * 0.0054931640625f);
        DbgPrintf(3, 8, dbgObjStrScl, obj->scale[0], obj->scale[1], obj->scale[2]);
        part = ChrModelGetPart(obj, dbgObjPartsNo);
    }
    if (part != NULL) {
        DbgPrintf(3, 9, dbgObjStrPartsNoFlagId, dbgObjPartsNo, part->flags, part->id);
        DbgPrintf(3, 10, dbgObjStrPos, part->pos[0], part->pos[1], part->pos[2]);
        DbgPrintf(3, 11, dbgObjStrClp, part->cullSphere[0], part->cullSphere[1], part->cullSphere[2], part->cullSphere[3]);
        DbgPrintf(3, 12, dbgObjStrAng, part->rot[0] * 0.0054931640625f, part->rot[1] * 0.0054931640625f,
                      part->rot[2] * 0.0054931640625f);
        DbgPrintf(3, 13, dbgObjStrScl, part->scale[0], part->scale[1], part->scale[2]);
        DbgPrintf(15, 14, dbgObjStrMax, ChrPartGetGaObjCount(part));
    } else {
        DbgPrintf(3, 9, dbgObjStrPartsNo, dbgObjPartsNo);
    }
    DbgPrintf(3, 14, dbgObjStrGaObjNo, dbgObjGaObjNo);
    if (sub != NULL) {
        DbgPrintf(3, 15, dbgObjStrAlpha, sub->x50);
        DbgPrintf(3, 16, dbgObjStrVertex, sub->x48);
        DbgPrintf(17, 16, dbgObjStrPolygon, sub->x44);
        DbgPrintf(3, 17, dbgObjStrClipStat, sub->x2);
        DbgPrintf(3, 18, dbgObjStrCenter, sub->x10, sub->x14, sub->x18);
        DbgPrintf(3, 19, dbgObjStrRangeTexId, sub->x20, sub->x8);
    }
}
