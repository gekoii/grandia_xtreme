#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "gph.h"
#include "main/n02.h"

void GphEnvMapSetColor(s32, DbgColor *);

extern s32 dbgPartsAtrLine;
extern s32 dbgPartsAtrCursor;
extern s32 dbgPartsAtrPartNo;
extern char dbgPartsAtrStrPartsAtrMenu[];
extern char dbgPartsAtrStrPartsNo[];
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
extern char dbgPartsAtrStrFogOn[];
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

void DbgPartsAtrPage(s32 idx) {
    void *model;
    GphPart *node = NULL;
    s32 v;
    s32 h;

    if (dbgObjList == NULL) {
        return;
    }
    model = dbgObjList->objs[idx];
    if (model != NULL) {
        while (ChrModelGetPart(model, (s32)node) != NULL) {
            node = (GphPart *)((s32)node + 1);
        }
    }
    dbgPad = &padPort1;
    node = NULL;
    if (model != NULL) {
        node = ChrModelGetPart(model, dbgPartsAtrPartNo);
    }
    if (!(dbgPad->held & 2)) {
        dbgPartsAtrLine = DbgMenuLineSelect(dbgPartsAtrLine, 24);
        switch (dbgPartsAtrLine) {
        case 0:
            dbgPartsAtrCursor = dbgPartsAtrLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgPartsAtrPartNo--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgPartsAtrPartNo++;
            }
            if (dbgPartsAtrPartNo < 0) {
                dbgPartsAtrPartNo = 0;
            }
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            if (node == NULL) {
                dbgPartsAtrPartNo = 0;
                return;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 10:
            dbgPartsAtrCursor = dbgPartsAtrLine;
            if (model != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    node = ChrModelGetPart(model, dbgPartsAtrPartNo);
                    ChrPartSetAttr(node, dbgPartsAtrLine - 1, 0);
                }
                if (dbgPad->repeatFast & 0x2000) {
                    node = ChrModelGetPart(model, dbgPartsAtrPartNo);
                    ChrPartSetAttr(node, dbgPartsAtrLine - 1, 1);
                }
            }
            break;
        case 13:
            dbgPartsAtrCursor = dbgPartsAtrLine;
            if (model != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    node = ChrModelGetPart(model, dbgPartsAtrPartNo);
                    ChrPartSetAttr(node, 13, 0);
                }
                if (dbgPad->repeatFast & 0x2000) {
                    node = ChrModelGetPart(model, dbgPartsAtrPartNo);
                    ChrPartSetAttr(node, 13, 1);
                }
            }
            break;
        case 1:
            dbgPartsAtrCursor = dbgPartsAtrLine;
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            v = ChrPartGetAttr(node, 0);
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
            ChrPartSetAttr(node, 0, v);
            break;
        case 9:
            dbgPartsAtrCursor = dbgPartsAtrLine;
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            v = ChrPartGetAttr(node, 8);
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
            ChrPartSetAttr(node, 8, v);
            break;
        case 11:
            dbgPartsAtrCursor = dbgPartsAtrLine;
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            v = ChrPartGetAttr(node, 10);
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
            ChrPartSetAttr(node, 10, v);
            break;
        case 12:
            dbgPartsAtrCursor = dbgPartsAtrLine;
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            v = ChrPartGetAttr(node, 11);
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
            ChrPartSetAttr(node, 11, v);
            break;
        case 14:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            v = node->alphaFix;
            if (dbgPad->repeatFast & 0x8000) {
                if (v != 0) {
                    v--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                v++;
            }
            node->alphaFix = v;
            break;
        case 15:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            h = node->uvScroll[1];
            if (dbgPad->repeatFast & 0x8000) {
                h--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                h++;
            }
            node->uvScroll[1] = h;
            break;
        case 16:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            node = ChrModelGetPart(model, dbgPartsAtrPartNo);
            h = node->uvScroll[3];
            if (dbgPad->repeatFast & 0x8000) {
                h--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                h++;
            }
            node->uvScroll[3] = h;
            break;
        case 17:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->color[0] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->color[0] += 1.0f;
            }
            break;
        case 18:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->color[1] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->color[1] += 1.0f;
            }
            break;
        case 19:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->color[2] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->color[2] += 1.0f;
            }
            break;
        case 20:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->color[3] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->color[3] += 1.0f;
            }
            break;
        case 21:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->prio--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->prio++;
            }
            node->prio &= 0xF;
            break;
        case 22:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                if (node->attr & 0x80) {
                    node->attr ^= 0x80;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->attr |= 0x80;
            }
            break;
        case 23:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                dbgEnvMapParam.a -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgEnvMapParam.a += 1.0f;
            }
            GphEnvMapSetColor(0, &dbgEnvMapParam);
            break;
        case 24:
            dbgPartsAtrCursor = dbgPartsAtrLine - 14;
            if (dbgPad->repeatFast & 0x8000) {
                node->flags |= 8;
            }
            if (dbgPad->repeatFast & 0x2000) {
                node->flags = ~8;
            }
            break;
        }
    }
    if (dbgPartsAtrLine >= 14) {
        DbgMenuCursorDraw(0x16, dbgPartsAtrCursor + 5);
    } else {
        DbgMenuCursorDraw(2, dbgPartsAtrCursor + 5);
    }
    DbgPuts(2, 3, dbgPartsAtrStrPartsAtrMenu);
    DbgPrintf(3, 5, dbgPartsAtrStrPartsNo, dbgPartsAtrPartNo);
    if (node == NULL) {
        return;
    }
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
        DbgPrintf(3, 18, dbgPartsAtrStrFogOn);
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
    if (node->flags & 8) {
        DbgPrintf(0x17, 15, dbgAtrStrDispOff);
    } else {
        DbgPrintf(0x17, 15, dbgAtrStrDispOn);
    }
}
