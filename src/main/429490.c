#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "gph.h"
#include "main/n02.h"

void GphEnvMapSetColor(s32, DbgColor *);

extern s32 dbgObjAtrLine;
extern s32 dbgObjAtrCursor;
extern char dbgObjAtrStrObjectNo[];
extern char dbgObjAtrStrAttribCtrlMode[];
extern char dbgAtrStrBillboard[];
extern char dbgAtrStrAmbientOnlyOn[];
extern char dbgAtrStrAmbientOnlyOff[];
extern char dbgAtrStrIgnoreLightOn[];
extern char dbgAtrStrIgnoreLightOff[];
extern char dbgAtrStrUvScroolOn[];
extern char dbgAtrStrUvScroolOff[];
extern char dbgAtrStrTextureOn[];
extern char dbgAtrStrTextureOff[];
extern char dbgAtrStrUClampOn[];
extern char dbgAtrStrUClampOff[];
extern char dbgAtrStrVClampOn[];
extern char dbgAtrStrVClampOff[];
extern char dbgAtrStrAlphaTestOn[];
extern char dbgAtrStrAlphaTestOff[];
extern char dbgAtrStrAlphaMode[];
extern char dbgAtrStrZTestOn[];
extern char dbgAtrStrZTestOff[];
extern char dbgAtrStrZTestMode[];
extern char dbgAtrStrAlphaBlend[];
extern char dbgObjAtrStrFogOn[];
extern char dbgAtrStrFogOff[];
extern char dbgAtrStrAlphaFix[];
extern char dbgAtrStrUScrStep[];
extern char dbgAtrStrVScrStep[];
extern char dbgAtrStrSurfaceR[];
extern char dbgAtrStrSurfaceG[];
extern char dbgAtrStrSurfaceB[];
extern char dbgAtrStrSurfaceA[];
extern char dbgAtrStrPartsPrio[];
extern char dbgAtrStrEnvmapOn[];
extern char dbgAtrStrEnvmapOff[];
extern char dbgAtrStrEnvmapA[];
extern char dbgAtrStrDispOff[];
extern char dbgAtrStrDispOn[];

s32 DbgObjAtrPage(s32 idx) {
    GphModel *model;
    GphPart *node = NULL;
    u32 count = 0;
    u32 i;
    s32 v;
    f32 f;

    if (dbgObjList == NULL) {
        return idx;
    }
    model = dbgObjList->objs[idx];
    if (model != NULL) {
        while (ChrModelGetPart(model, count) != NULL) {
            count++;
        }
    }
    dbgPad = &padPort1;
    node = NULL;
    if (model != NULL) {
        node = ChrModelGetPart(model, 0);
    }
    if (!(dbgPad->held & 2)) {
        dbgObjAtrLine = DbgMenuLineSelect(dbgObjAtrLine, 24);
        switch (dbgObjAtrLine) {
        case 0:
            dbgObjAtrCursor = dbgObjAtrLine;
            if (dbgPad->repeatFast & 0x8000) {
                idx--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                idx++;
            }
            if (idx < 0) {
                idx = 0;
            }
            model = dbgObjList->objs[idx];
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 10:
            dbgObjAtrCursor = dbgObjAtrLine;
            if (model != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    for (i = 0; i < count; i++) {
                        ChrPartSetAttr(ChrModelGetPart(model, i), dbgObjAtrLine - 1, 0);
                    }
                }
                if (dbgPad->repeatFast & 0x2000) {
                    for (i = 0; i < count; i++) {
                        ChrPartSetAttr(ChrModelGetPart(model, i), dbgObjAtrLine - 1, 1);
                    }
                }
            }
            break;
        case 13:
            dbgObjAtrCursor = dbgObjAtrLine;
            if (model != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    for (i = 0; i < count; i++) {
                        ChrPartSetAttr(ChrModelGetPart(model, i), 13, 0);
                    }
                }
                if (dbgPad->repeatFast & 0x2000) {
                    for (i = 0; i < count; i++) {
                        ChrPartSetAttr(ChrModelGetPart(model, i), 13, 1);
                    }
                }
            }
            break;
        case 1:
            dbgObjAtrCursor = dbgObjAtrLine;
            v = ChrPartGetAttr(ChrModelGetPart(model, 0), 0);
            if (dbgPad->repeatFast & 0x8000) {
                if (v != 0) {
                    v--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                if (v <= 4) {
                    v++;
                }
            }
            for (i = 0; i < count; i++) {
                ChrPartSetAttr(ChrModelGetPart(model, i), 0, v);
            }
            break;
        case 9:
            dbgObjAtrCursor = dbgObjAtrLine;
            v = ChrPartGetAttr(ChrModelGetPart(model, 0), 8);
            if (dbgPad->repeatFast & 0x8000) {
                if (v != 0) {
                    v--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                if (v <= 6) {
                    v++;
                }
            }
            for (i = 0; i < count; i++) {
                ChrPartSetAttr(ChrModelGetPart(model, i), 8, v);
            }
            break;
        case 11:
            dbgObjAtrCursor = dbgObjAtrLine;
            v = ChrPartGetAttr(ChrModelGetPart(model, 0), 10);
            if (dbgPad->repeatFast & 0x8000) {
                if (v != 0) {
                    v--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                if (v <= 2) {
                    v++;
                }
            }
            for (i = 0; i < count; i++) {
                ChrPartSetAttr(ChrModelGetPart(model, i), 10, v);
            }
            break;
        case 12:
            dbgObjAtrCursor = dbgObjAtrLine;
            v = ChrPartGetAttr(ChrModelGetPart(model, 0), 11);
            if (dbgPad->repeatFast & 0x8000) {
                if (v != 0) {
                    v--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                if (v <= 1) {
                    v++;
                }
            }
            for (i = 0; i < count; i++) {
                ChrPartSetAttr(ChrModelGetPart(model, i), 11, v);
            }
            break;
        case 14:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            v = ChrModelGetPart(model, 0)->alphaFix;
            if (dbgPad->repeatFast & 0x8000) {
                if (v != 0) {
                    v--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                v++;
            }
            for (i = 0; i < count; i++) {
                ChrModelGetPart(model, i)->alphaFix = v;
            }
            break;
        case 15:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            v = ChrModelGetPart(model, 0)->uvScroll[1];
            if (dbgPad->repeatFast & 0x8000) {
                v--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                v++;
            }
            for (i = 0; i < count; i++) {
                ChrModelGetPart(model, i)->uvScroll[1] = v;
            }
            break;
        case 16:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            v = ChrModelGetPart(model, 0)->uvScroll[3];
            if (dbgPad->repeatFast & 0x8000) {
                v--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                v++;
            }
            for (i = 0; i < count; i++) {
                ChrModelGetPart(model, i)->uvScroll[3] = v;
            }
            break;
        case 17:
        case 18:
        case 19:
        case 20:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->color[dbgObjAtrLine - 17] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->color[dbgObjAtrLine - 17] += 1.0f;
            }
            f = node->color[dbgObjAtrLine - 17];
            for (i = 0; i < count; i++) {
                ChrModelGetPart(model, i)->color[dbgObjAtrLine - 17] = f;
            }
            break;
        case 21:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->prio--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->prio++;
            }
            node->prio &= 0xF;
            f = node->prio;
            for (i = 0; i < count; i++) {
                ChrModelGetPart(model, i)->prio = f;
            }
            break;
        case 22:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                for (i = 0; i < count; i++) {
                    node = ChrModelGetPart(model, i);
                    if (node->attr & 0x80) {
                        node->attr ^= 0x80;
                    }
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                for (i = 0; i < count; i++) {
                    node = ChrModelGetPart(model, i);
                    node->attr |= 0x80;
                }
            }
            break;
        case 23:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                dbgEnvMapParam.a -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgEnvMapParam.a += 1.0f;
            }
            GphEnvMapSetColor(0, &dbgEnvMapParam);
            break;
        case 24:
            dbgObjAtrCursor = dbgObjAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                model->flags |= 8;
                for (i = 0; i < count; i++) {
                    node = ChrModelGetPart(model, i);
                    node->flags |= 8;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                model->flags = ~8;
                for (i = 0; i < count; i++) {
                    ChrModelGetPart(model, i)->flags = ~8;
                }
            }
            break;
        }
    }
    if (dbgObjAtrLine >= 14) {
        DbgMenuCursorDraw(0x16, dbgObjAtrCursor + 5);
    } else {
        DbgMenuCursorDraw(2, dbgObjAtrCursor + 5);
    }
    DbgPrintf(3, 5, dbgObjAtrStrObjectNo, idx);
    if (model == NULL) {
        return idx;
    }
    node = ChrModelGetPart(model, 0);
    if (node == NULL) {
        return idx;
    }
    DbgPrintf(3, 4, dbgObjAtrStrAttribCtrlMode, node->attr, node->gsFlags);
    DbgPrintf(3, 6, dbgAtrStrBillboard, ChrPartGetAttr(node, 0));
    if (ChrPartGetAttr(node, 1)) {
        DbgPrintf(3, 7, dbgAtrStrAmbientOnlyOn);
    } else {
        DbgPrintf(3, 7, dbgAtrStrAmbientOnlyOff);
    }
    if (ChrPartGetAttr(node, 2)) {
        DbgPrintf(3, 8, dbgAtrStrIgnoreLightOn);
    } else {
        DbgPrintf(3, 8, dbgAtrStrIgnoreLightOff);
    }
    if (ChrPartGetAttr(node, 3)) {
        DbgPrintf(3, 9, dbgAtrStrUvScroolOn);
    } else {
        DbgPrintf(3, 9, dbgAtrStrUvScroolOff);
    }
    if (ChrPartGetAttr(node, 4)) {
        DbgPrintf(3, 10, dbgAtrStrTextureOn);
    } else {
        DbgPrintf(3, 10, dbgAtrStrTextureOff);
    }
    if (ChrPartGetAttr(node, 5)) {
        DbgPrintf(3, 11, dbgAtrStrUClampOn);
    } else {
        DbgPrintf(3, 11, dbgAtrStrUClampOff);
    }
    if (ChrPartGetAttr(node, 6)) {
        DbgPrintf(3, 12, dbgAtrStrVClampOn);
    } else {
        DbgPrintf(3, 12, dbgAtrStrVClampOff);
    }
    if (ChrPartGetAttr(node, 7)) {
        DbgPrintf(3, 13, dbgAtrStrAlphaTestOn);
    } else {
        DbgPrintf(3, 13, dbgAtrStrAlphaTestOff);
    }
    DbgPrintf(3, 14, dbgAtrStrAlphaMode, ChrPartGetAttr(node, 8));
    if (ChrPartGetAttr(node, 9)) {
        DbgPrintf(3, 15, dbgAtrStrZTestOn);
    } else {
        DbgPrintf(3, 15, dbgAtrStrZTestOff);
    }
    DbgPrintf(3, 16, dbgAtrStrZTestMode, ChrPartGetAttr(node, 10));
    DbgPrintf(3, 17, dbgAtrStrAlphaBlend, ChrPartGetAttr(node, 11));
    if (ChrPartGetAttr(node, 13)) {
        DbgPrintf(3, 18, dbgObjAtrStrFogOn);
    } else {
        DbgPrintf(3, 18, dbgAtrStrFogOff);
    }
    DbgPrintf(0x17, 5, dbgAtrStrAlphaFix, node->alphaFix);
    DbgPrintf(0x17, 6, dbgAtrStrUScrStep, node->uvScroll[1]);
    DbgPrintf(0x17, 7, dbgAtrStrVScrStep, node->uvScroll[3]);
    DbgPrintf(0x17, 8, dbgAtrStrSurfaceR, node->color[0]);
    DbgPrintf(0x17, 9, dbgAtrStrSurfaceG, node->color[1]);
    DbgPrintf(0x17, 10, dbgAtrStrSurfaceB, node->color[2]);
    DbgPrintf(0x17, 11, dbgAtrStrSurfaceA, node->color[3]);
    DbgPrintf(0x17, 12, dbgAtrStrPartsPrio, node->prio);
    if (node->attr & 0x80) {
        DbgPrintf(0x17, 13, dbgAtrStrEnvmapOn);
    } else {
        DbgPrintf(0x17, 13, dbgAtrStrEnvmapOff);
    }
    DbgPrintf(0x17, 14, dbgAtrStrEnvmapA, (s32)dbgEnvMapParam.a);
    if (model->flags & 8) {
        DbgPrintf(0x17, 15, dbgAtrStrDispOff);
    } else {
        DbgPrintf(0x17, 15, dbgAtrStrDispOn);
    }
    return idx;
}
