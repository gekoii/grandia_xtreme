/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/state.h"
#include "field/n16.h"

extern s32 dbgFldCamInfoLine;
extern s32 dbgFldCamTarget;
extern char dbgFldCamAiNames[][16];
extern char dbgCamScrStrPosX[];
extern char dbgCamScrStrY[];
extern char dbgCamScrStrZ[];
extern char dbgCamScrStrMoveX[];
extern char dbgCamScrStrTurnX[];
extern char dbgCamScrStrLength[];
extern char dbgCamScrStrPers[];
extern char dbgCamScrStrZoom[];
extern char dbgCamScrStrClipF[];
extern char dbgCamScrStrAi[];
extern char dbgCamScrStrTargetPlayer[];
extern char dbgCamScrStrTargetTenant[];


void DbgFldCamScriptInfoPage(void) {
    Pad *pad;

    pad = &padPort1;
    DbgPrintf(4, 5, dbgCamScrStrPosX, camField[0].eye[0]);
    DbgPrintf(4, 6, dbgCamScrStrY, camField[0].eye[1]);
    DbgPrintf(4, 7, dbgCamScrStrZ, camField[0].eye[2]);
    DbgPrintf(4, 8, dbgCamScrStrMoveX, camField[0].lookAt[0]);
    DbgPrintf(4, 9, dbgCamScrStrY, camField[0].lookAt[1]);
    DbgPrintf(4, 10, dbgCamScrStrZ, camField[0].lookAt[2]);
    DbgPrintf(4, 11, dbgCamScrStrTurnX, camField[0].pitch * 57.295776f);
    DbgPrintf(4, 12, dbgCamScrStrY, camField[0].yaw * 57.295776f);
    DbgPrintf(4, 13, dbgCamScrStrZ, camField[0].roll * 57.295776f);
    DbgPrintf(4, 14, dbgCamScrStrLength, camField[0].dist);
    DbgPrintf(4, 15, dbgCamScrStrPers, camField[0].fov);
    DbgPrintf(4, 16, dbgCamScrStrZoom, camField[0].zoom);
    DbgPrintf(4, 17, dbgCamScrStrClipF, camField[0].clipFar);
    DbgPrintf(4, 18, dbgCamScrStrAi, dbgFldCamAiNames[camCtl.aiType]);
    DbgPrintf(24, 5, dbgMainFlagStrCameraPause, dbgFldRunPauseStr[EvtSystemFlagGet(0x82)]);
    DbgPrintf(24, 6, dbgMainFlagStrCameraStatus, dbgFldStopRunStr[EvtSystemFlagGet(0x83)]);
    if (dbgFldCamTarget == 0) {
        DbgPrintf(4, 19, dbgCamScrStrTargetPlayer);
    } else {
        DbgPrintf(4, 19, dbgCamScrStrTargetTenant, dbgFldCamTarget - 1);
    }
    if (dbgFldCamInfoLine < 15) {
        DbgDrawCursor(3, dbgFldCamInfoLine + 5);
    } else {
        DbgDrawCursor(23, dbgFldCamInfoLine - 10);
    }
    if (!(pad->held & 2)) {
        if (pad->repeatFast & 0x1000) {
            if (dbgFldCamInfoLine == 0) {
                dbgFldCamInfoLine = 16;
            } else {
                dbgFldCamInfoLine--;
            }
        }
        if (pad->repeatFast & 0x4000) {
            if (dbgFldCamInfoLine < 16) {
                dbgFldCamInfoLine++;
            } else {
                dbgFldCamInfoLine = 0;
            }
        }
        switch (dbgFldCamInfoLine) {
        case 0:
        case 1:
        case 2:
            if (pad->repeatFast & 0x8000) {
                camField[0].eye[dbgFldCamInfoLine] -= 10.0f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].eye[dbgFldCamInfoLine] += 10.0f;
            }
            break;
        case 3:
        case 4:
        case 5:
            if (pad->repeatFast & 0x8000) {
                camField[0].lookAt[dbgFldCamInfoLine - 3] -= 10.0f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].lookAt[dbgFldCamInfoLine - 3] += 10.0f;
            }
            break;
        case 6:
            if (pad->repeatFast & 0x8000) {
                camField[0].pitch -= 0.017453292f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].pitch += 0.017453292f;
            }
            break;
        case 7:
            if (pad->repeatFast & 0x8000) {
                camField[0].yaw -= 0.017453292f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].yaw += 0.017453292f;
            }
            break;
        case 8:
            if (pad->repeatFast & 0x8000) {
                camField[0].roll -= 0.017453292f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].roll += 0.017453292f;
            }
            break;
        case 9:
            if (pad->repeatFast & 0x8000) {
                camField[0].dist -= 10.0f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].dist += 10.0f;
            }
            if (camField[0].dist < 0.0f) {
                camField[0].dist = 0.0f;
            }
            break;
        case 10:
            if (pad->repeatFast & 0x8000) {
                camField[0].fov -= 1.0f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].fov += 1.0f;
            }
            if (camField[0].fov < 1.0f) {
                camField[0].fov = 1.0f;
            }
            break;
        case 11:
            if (pad->repeatFast & 0x8000) {
                camField[0].zoom -= 0.01f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].zoom += 0.01f;
            }
            if (camField[0].zoom < 0.01f) {
                camField[0].zoom = 0.01f;
            }
            break;
        case 12:
            if (pad->repeatFast & 0x8000) {
                camField[0].clipFar -= 1.0f;
            }
            if (pad->repeatFast & 0x2000) {
                camField[0].clipFar += 1.0f;
            }
            break;
        case 13:
            if ((pad->repeatFast & 0x8000) && camCtl.aiType != 0) {
                camCtl.aiType--;
            }
            if ((pad->repeatFast & 0x2000) && camCtl.aiType < 13) {
                camCtl.aiType++;
            }
            break;
        case 14:
            if (pad->repeatFast & 0x8000) {
                dbgFldCamTarget--;
            }
            if (pad->repeatFast & 0x2000) {
                dbgFldCamTarget++;
            }
            if (dbgFldCamTarget < 0) {
                dbgFldCamTarget = ChrTenantGetCount();
            }
            if (ChrTenantGetCount() < dbgFldCamTarget) {
                dbgFldCamTarget = 0;
            }
            if (dbgFldCamTarget == 0) {
                camCtl.target = (ChrObj *)ChrGetByNo(1);
            } else {
                camCtl.target = ChrTenantAt(dbgFldCamTarget - 1);
            }
            break;
        case 15:
            if (pad->repeatFast & 0x2000) {
                EvtSystemFlagSet(0x82, 1);
            }
            if (pad->repeatFast & 0x8000) {
                EvtSystemFlagSet(0x82, 0);
            }
            break;
        case 16:
            if (pad->repeatFast & 0x2000) {
                EvtSystemFlagSet(0x83, 1);
            }
            if (pad->repeatFast & 0x8000) {
                EvtSystemFlagSet(0x83, 0);
            }
            break;
        }
    }
    camCtl.prevType = camCtl.aiType;
}
