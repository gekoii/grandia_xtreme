/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "camera.h"
#include "gph.h"
#include "main/n02.h"
#include "chr.h"
#include "protos/main.h"

extern s32 dbgMotLine;
extern s32 dbgMotCursor;
extern s32 dbgMotMotionNo;
extern char dbgMotObjStrDataMax[];
extern char dbgMotObjStrObj[];
extern char dbgMotObjStrRun[];
extern char dbgMotObjStrStop[];
extern char dbgMotObjStrSs[];
extern char dbgMotObjStrMot0Stop[];
extern char dbgMotObjStrMot0[];
extern char dbgMotObjStrFrm[];
extern char dbgMotObjStrSpd[];
extern char dbgMotObjStrLoopOn[];
extern char dbgMotObjStrLoopOff[];
extern char dbgMotObjStrDashes[];
extern char dbgMotObjStrCrsOn[];
extern char dbgMotObjStrCrsOff[];
extern char dbgMotObjStrSdwOn[];
extern char dbgMotObjStrSdwOff[];
extern char dbgMotObjStrStgOn[];
extern char dbgMotObjStrStgOff[];
extern char dbgMotObjStrPosX[];
extern char dbgMotObjStrRotY[];
extern char dbgMotObjStrRef[];
extern char dbgMotObjStrSmt[];

void GphEnvMapSetColor(s32, DbgColor *);

void DbgMotionObjPage(void) {
    GphModel *obj;
    GphMotion *anim;
    u16 last;
    s32 i;
    Camera *cam;
    GphPart *part;

    dbgPad = &padPort1;
    obj = dbgObjList->objs[dbgObjNo];
    if (obj == NULL) {
        return;
    }
    anim = obj->motion[0].motion;
    if (anim == NULL) {
        last = 0;
    } else {
        last = anim->frameCount - 1;
    }
    DbgMenuCursorDraw(0, dbgMotCursor + 5);
    if (!(dbgPad->held & 2)) {
        switch (dbgMotLine = DbgMenuLineSelect(dbgMotLine, 12)) {
        case 0:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgObjNo--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgObjNo++;
            }
            if (dbgObjNo < 0) {
                dbgObjNo = dbgObjList->count - 1;
            }
            if (dbgObjNo >= dbgObjList->count) {
                dbgObjNo = 0;
            }
            if (dbgPad->repeatFast & 0x20) {
                cam = dbgCamera;
                cam->lookAt[0] = obj->pos[0];
                cam->lookAt[1] = obj->pos[1] + 120.0f;
                cam->lookAt[2] = obj->pos[2];
                cam->yaw = cam->roll = 0.0f;
            }
            break;
        case 1:
            dbgMotCursor = dbgMotLine;
            if ((dbgPad->repeatFast & 0x8000) && dbgMotObjParams[dbgObjNo].mode != 0) {
                dbgMotObjParams[dbgObjNo].mode--;
            }
            if ((dbgPad->repeatFast & 0x2000) && dbgMotObjParams[dbgObjNo].mode < 2) {
                dbgMotObjParams[dbgObjNo].mode++;
            }
            if (dbgPad->repeatFast & 0x20) {
                obj->motion[0].frame = 0.0f;
            }
            break;
        case 5:
            dbgMotCursor = dbgMotLine;
            if (anim != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    anim->playFlags = 0;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    anim->playFlags = 1;
                }
            }
            break;
        case 4:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgMotObjParams[dbgObjNo].speed -= 0.01f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgMotObjParams[dbgObjNo].speed += 0.01f;
            }
            if (dbgMotObjParams[dbgObjNo].speed < 0.0f) {
                dbgMotObjParams[dbgObjNo].speed = 0.0f;
            }
            break;
        case 3:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->motion[0].frame = obj->motion[0].frame - dbgMotObjParams[dbgObjNo].speed;
                if (obj->motion[0].frame < 0.0f) {
                    obj->motion[0].frame = last;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->motion[0].frame = obj->motion[0].frame + dbgMotObjParams[dbgObjNo].speed;
                if (last <= obj->motion[0].frame) {
                    obj->motion[0].frame = 0.0f;
                }
            }
            break;
        case 2:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                if (dbgMotMotionNo >= 0) {
                    dbgMotMotionNo--;
                }
                dbgMotObjParams[dbgObjNo].motionNo = dbgMotMotionNo;
                obj->motion[0].frame = 0.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                if (++dbgMotMotionNo >= (s32)chrMotionList.count) { /* count is u32: signed compare */
                    dbgMotMotionNo = chrMotionList.count - 1;
                }
                dbgMotObjParams[dbgObjNo].motionNo = dbgMotMotionNo;
                obj->motion[0].frame = 0.0f;
            }
            if (dbgPad->repeatFast & 0x20) {
                dbgMotObjParams[dbgObjNo].motionNo = dbgMotMotionNo;
                obj->motion[0].frame = 0.0f;
            }
            break;
        case 6:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgMotObjParams[0].flag = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgMotObjParams[0].flag = 1;
            }
            break;
        case 7:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgMotObjParams[1].flag = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgMotObjParams[1].flag = 1;
            }
            break;
        case 8:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgMotObjParams[2].flag = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgMotObjParams[2].flag = 1;
            }
            break;
        case 9:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->pos[0] -= 4.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->pos[0] += 4.0f;
            }
            break;
        case 10:
            dbgMotCursor = dbgMotLine;
            if (dbgCamera != NULL) {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgCamera->yaw -= 0.7853982f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgCamera->yaw += 0.7853982f;
                }
            }
            break;
        case 11:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgEnvMapParam.a -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgEnvMapParam.a += 1.0f;
            }
            GphEnvMapSetColor(0, &dbgEnvMapParam);
            if (obj != NULL) {
                if ((s32)dbgEnvMapParam.a != 0) {
                    for (i = 0; i < obj->partCount; i++) {
                        part = ChrModelGetPart(obj, i);
                        part->attr |= 0x80;
                    }
                } else {
                    for (i = 0; i < obj->partCount; i++) {
                        part = ChrModelGetPart(obj, i);
                        if (part->attr & 0x80) {
                            part->attr ^= 0x80;
                        }
                    }
                }
            }
            break;
        case 12:
            dbgMotCursor = dbgMotLine;
            if (dbgPad->repeatFast & 0x8000) {
                dbgMotObjParams[3].flag--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgMotObjParams[3].flag++;
            }
            break;
        }
    }
    DbgPrintf(1, 3, dbgMotObjStrDataMax, chrMotionList.count);
    DbgPrintf(1, 5, dbgMotObjStrObj, dbgObjNo);
    switch (dbgMotObjParams[dbgObjNo].mode) {
    case 1:
        DbgPrintf(1, 6, dbgMotObjStrRun);
        break;
    case 0:
        DbgPrintf(1, 6, dbgMotObjStrStop);
        break;
    case 2:
        DbgPrintf(1, 6, dbgMotObjStrSs);
        break;
    }
    if (dbgMotMotionNo == -1) {
        DbgPrintf(1, 7, dbgMotObjStrMot0Stop, chrMotionList.count);
    } else {
        DbgPrintf(1, 7, dbgMotObjStrMot0, dbgMotMotionNo);
    }
    DbgPrintf(1, 8, dbgMotObjStrFrm, obj->motion[0].frame);
    DbgPrintf(1, 9, dbgMotObjStrSpd, dbgMotObjParams[dbgObjNo].speed);
    if (anim != NULL) {
        if (anim->playFlags == 1) {
            DbgPrintf(1, 10, dbgMotObjStrLoopOn);
        } else {
            DbgPrintf(1, 10, dbgMotObjStrLoopOff);
        }
    } else {
        DbgPrintf(1, 10, dbgMotObjStrDashes);
    }
    if (dbgMotObjParams[0].flag != 0) {
        DbgPrintf(1, 11, dbgMotObjStrCrsOn);
    } else {
        DbgPrintf(1, 11, dbgMotObjStrCrsOff);
    }
    if (dbgMotObjParams[1].flag != 0) {
        DbgPrintf(1, 12, dbgMotObjStrSdwOn);
    } else {
        DbgPrintf(1, 12, dbgMotObjStrSdwOff);
    }
    if (dbgMotObjParams[2].flag != 0) {
        DbgPrintf(1, 13, dbgMotObjStrStgOn);
    } else {
        DbgPrintf(1, 13, dbgMotObjStrStgOff);
    }
    DbgPrintf(1, 14, dbgMotObjStrPosX, (s32)obj->pos[0]);
    DbgPrintf(1, 15, dbgMotObjStrRotY);
    DbgPrintf(1, 16, dbgMotObjStrRef, (s32)dbgEnvMapParam.a);
    DbgPrintf(1, 17, dbgMotObjStrSmt, dbgMotObjParams[3].flag);
}
