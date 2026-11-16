/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "camera.h"
#include "gph.h"
#include "main/n02.h"
#include "protos/main.h"

extern void VuDirFromAngles(f32 *, DbgLightAngle *);
extern s32 dbgEnvLine;
extern s32 dbgEnvLightNo;
extern s32 dbgEnvCursor;
extern u8 dbgEnvStrEnviromentsMenu[];
extern u8 dbgEnvStrLightNoTypeAtr[];
extern u8 dbgEnvStrPColorR[];
extern u8 dbgEnvStrPColorG[];
extern u8 dbgEnvStrPColorB[];
extern u8 dbgEnvStrPColorA[];
extern u8 dbgEnvStrAngleX[];
extern u8 dbgEnvStrY[];
extern u8 dbgEnvStrPower[];
extern u8 dbgEnvStrPosX[];
extern u8 dbgEnvStrZ[];
extern u8 dbgEnvStrAmbientR[];
extern u8 dbgEnvStrBgColR[];
extern u8 dbgEnvStrBgColG[];
extern u8 dbgEnvStrBgColB[];
extern u8 dbgEnvStrFogR[];
extern u8 dbgEnvStrFogDist[];
extern u8 dbgEnvStrFogLimit[];
extern u8 dbgEnvStrFogAdd[];
extern u8 dbgEnvStrClipNear[];
extern u8 dbgEnvStrClipFar[];
extern u8 dbgEnvStrViewangle[];
extern u8 dbgEnvStrFrameMode[];
extern u8 dbgEnvStrFrameSkipOn[];
extern u8 dbgEnvStrFrameSkipOff[];

void DbgEnvPage(void) {
    f32 vec[4];
    GphLight *obj;
    s32 idx;
    s32 t;
    s32 r;
    s32 h;
    s32 m;
    s32 s;

    idx = 0;
    dbgPad = &padPort1;
    t = GsGetVblankCount() / 60;
    r = t / 3600;
    t = t - r * 3600;
    h = r;
    if (h < 1000) {
        m = t / 60;
        s = t - m * 60;
    } else {
        h = 999;
        m = 59;
        s = 59;
    }
    if (!(dbgPad->held & 2)) {
        dbgEnvLine = DbgMenuLineSelect(dbgEnvLine, 25);
        switch (dbgEnvLine) {
        case 0:
            dbgEnvCursor = 0;
            if (dbgPad->repeatFast & 0x8000) {
                dbgEnvLightNo--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgEnvLightNo++;
            }
            if (dbgEnvLightNo < 0) {
                dbgEnvLightNo = 0;
            }
            if (GphLightGet(dbgEnvLightNo + 1) == NULL) {
                dbgEnvLightNo = 0;
            }
            break;
        case 1:
            obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[0] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[0] += 1.0f;
            }
            break;
        case 2:
            obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[1] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[1] += 1.0f;
            }
            break;
        case 3:
            obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[2] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[2] += 1.0f;
            }
            break;
        case 4:
            obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[3] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[3] += 1.0f;
            }
            break;
        case 5:
            dbgEnvCursor = dbgEnvLine;
            obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
            if (obj->type == 0) {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgLightAngles[dbgEnvLightNo].x -= 1.0f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgLightAngles[dbgEnvLightNo].x += 1.0f;
                }
                if (dbgLightAngles[dbgEnvLightNo].x > 360.0f) {
                    dbgLightAngles[dbgEnvLightNo].x -= 360.0f;
                }
                if (dbgLightAngles[dbgEnvLightNo].x < -360.0f) {
                    dbgLightAngles[dbgEnvLightNo].x += 360.0f;
                }
                VuDirFromAngles(vec, &dbgLightAngles[dbgEnvLightNo]);
                obj->dir[0] = vec[0];
                obj->dir[1] = vec[1];
                obj->dir[2] = vec[2];
            }
            break;
        case 6:
            dbgEnvCursor = dbgEnvLine;
            obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
            if (obj->type == 0) {
                if (dbgPad->repeatFast & 0x8000) {
                    (&dbgLightAngles[dbgEnvLightNo])->y -= 1.0f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    (&dbgLightAngles[dbgEnvLightNo])->y += 1.0f;
                }
                if ((&dbgLightAngles[dbgEnvLightNo])->y > 360.0f) {
                    (&dbgLightAngles[dbgEnvLightNo])->y -= 360.0f;
                }
                if ((&dbgLightAngles[dbgEnvLightNo])->y < -360.0f) {
                    (&dbgLightAngles[dbgEnvLightNo])->y += 360.0f;
                }
                VuDirFromAngles(vec, &dbgLightAngles[dbgEnvLightNo]);
                obj->dir[0] = vec[0];
                obj->dir[1] = vec[1];
                obj->dir[2] = vec[2];
            }
            break;
        case 7:
            obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
            dbgEnvCursor = dbgEnvLine;
            if (obj->type == 0) {
                if (dbgPad->repeatFast & 0x8000) {
                    obj->intensity -= 0.1f;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    obj->intensity += 0.1f;
                }
            }
            break;
        case 8:
            obj = (GphLight *)GphLightGet(0);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[0] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[0] += 1.0f;
            }
            break;
        case 9:
            obj = (GphLight *)GphLightGet(0);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[1] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[1] += 1.0f;
            }
            break;
        case 10:
            obj = (GphLight *)GphLightGet(0);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[2] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[2] += 1.0f;
            }
            break;
        case 11:
            obj = (GphLight *)GphLightGet(0);
            dbgEnvCursor = dbgEnvLine;
            if (dbgPad->repeatFast & 0x8000) {
                obj->color[3] -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                obj->color[3] += 1.0f;
            }
            break;
        case 12:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgClearR--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgClearR++;
            }
            SysSetClearColor(0x80000000 | (dbgClearB << 16) | (dbgClearG << 8) | dbgClearR);
            break;
        case 13:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgClearG--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgClearG++;
            }
            SysSetClearColor(0x80000000 | (dbgClearB << 16) | (dbgClearG << 8) | dbgClearR);
            break;
        case 14:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgClearB--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgClearB++;
            }
            SysSetClearColor(0x80000000 | (dbgClearB << 16) | (dbgClearG << 8) | dbgClearR);
            break;
        case 15:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].fogColor[0]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].fogColor[0]++;
            }
            break;
        case 16:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].fogColor[1]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].fogColor[1]++;
            }
            break;
        case 17:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].fogColor[2]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].fogColor[2]++;
            }
            break;
        case 18:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].fogDist -= 50.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].fogDist += 50.0f;
            }
            break;
        case 19:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].fogMax -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].fogMax += 1.0f;
            }
            break;
        case 20:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].fogMin -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].fogMin += 1.0f;
            }
            break;
        case 21:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].clipNear -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].clipNear += 1.0f;
            }
            if (dbgCamera[idx].clipNear < 1.0f) {
                dbgCamera[idx].clipNear = 1.0f;
            }
            break;
        case 22:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].clipFar -= 50.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].clipFar += 50.0f;
            }
            break;
        case 23:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                dbgCamera[idx].fov -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgCamera[idx].fov += 1.0f;
            }
            break;
        case 24:
            dbgEnvCursor = dbgEnvLine - 11;
            if ((dbgPad->repeatFast & 0x8000) && sysState.frameMode != 0) {
                sysState.frameMode--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                sysState.frameMode++;
            }
            break;
        case 25:
            dbgEnvCursor = dbgEnvLine - 11;
            if (dbgPad->repeatFast & 0x8000) {
                sysState.frameSkip = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                sysState.frameSkip = 1;
            }
            break;
        }
    }
    if (dbgEnvLine >= 12) {
        DbgMenuCursorDraw(0x18, dbgEnvCursor + 5);
    } else {
        DbgMenuCursorDraw(2, dbgEnvCursor + 5);
    }
    DbgPrintf(2, 3, dbgEnvStrEnviromentsMenu, h, m, s);
    obj = (GphLight *)GphLightGet(dbgEnvLightNo + 1);
    if (obj != NULL) {
        DbgPrintf(3, 5, dbgEnvStrLightNoTypeAtr, dbgEnvLightNo, obj->type, obj->enable);
        DbgPrintf(3, 6, dbgEnvStrPColorR, obj->color[0]);
        DbgPrintf(3, 7, dbgEnvStrPColorG, obj->color[1]);
        DbgPrintf(3, 8, dbgEnvStrPColorB, obj->color[2]);
        DbgPrintf(3, 9, dbgEnvStrPColorA, obj->color[3]);
        if (obj->type == 0) {
            DbgPrintf(3, 10, dbgEnvStrAngleX, dbgLightAngles[dbgEnvLightNo].x);
            DbgPrintf(3, 11, dbgEnvStrY, (&dbgLightAngles[dbgEnvLightNo])->y);
            DbgPrintf(3, 12, dbgEnvStrPower, obj->intensity);
        } else {
            DbgPrintf(3, 10, dbgEnvStrPosX, obj->dir[0]);
            DbgPrintf(3, 11, dbgEnvStrY, obj->dir[1]);
            DbgPrintf(3, 12, dbgEnvStrZ, obj->dir[2]);
        }
    }
    obj = (GphLight *)GphLightGet(0);
    if (obj != NULL) {
        DbgPrintf(3, 13, dbgEnvStrAmbientR, obj->color[0]);
        DbgPrintf(3, 14, dbgEnvStrPColorG, obj->color[1]);
        DbgPrintf(3, 15, dbgEnvStrPColorB, obj->color[2]);
        DbgPrintf(3, 16, dbgEnvStrPColorA, obj->color[3]);
    }
    DbgPrintf(0x19, 6, dbgEnvStrBgColR, dbgClearR);
    DbgPrintf(0x19, 7, dbgEnvStrBgColG, dbgClearG);
    DbgPrintf(0x19, 8, dbgEnvStrBgColB, dbgClearB);
    DbgPrintf(0x19, 9, dbgEnvStrFogR, dbgCamera[idx].fogColor[0]);
    DbgPrintf(0x19, 10, dbgEnvStrBgColG, dbgCamera[idx].fogColor[1]);
    DbgPrintf(0x19, 11, dbgEnvStrBgColB, dbgCamera[idx].fogColor[2]);
    DbgPrintf(0x19, 12, dbgEnvStrFogDist, dbgCamera[idx].fogDist);
    DbgPrintf(0x19, 13, dbgEnvStrFogLimit, dbgCamera[idx].fogMax);
    DbgPrintf(0x19, 14, dbgEnvStrFogAdd, dbgCamera[idx].fogMin);
    DbgPrintf(0x19, 15, dbgEnvStrClipNear, dbgCamera[idx].clipNear);
    DbgPrintf(0x19, 16, dbgEnvStrClipFar, dbgCamera[idx].clipFar);
    DbgPrintf(0x19, 17, dbgEnvStrViewangle, dbgCamera[idx].fov);
    DbgPrintf(0x19, 18, dbgEnvStrFrameMode, sysState.frameMode);
    if (sysState.frameSkip != 0) {
        DbgPrintf(0x19, 19, dbgEnvStrFrameSkipOn);
    } else {
        DbgPrintf(0x19, 19, dbgEnvStrFrameSkipOff);
    }
}
