/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "camera.h"
#include "main/n02.h"
#include "protos/main.h"

extern u32 dbgCamLine;
extern s32 dbgCamCursor;
extern s32 dbgCamModeNo;
extern s32 dbgCamModes[];
extern char *dbgCamModeNames[];
extern char dbgCamStrCameraMenu[];
extern char dbgCamStrMode[];
extern char dbgCamStrPos[];
extern char dbgCamStrTarget[];
extern char dbgCamStrEyeVec[];
extern char dbgCamStrAngle[];
extern char dbgCamStrDst[];
extern char dbgCamStrZoom[];
extern char dbgCamStrQuakeTime[];
extern char dbgCamStrNextCycl[];
extern char dbgCamStrBase[];
extern char dbgCamStrRndm[];
extern char dbgCamStrCycl[];
extern char dbgCamStrBaseTrgt[];
extern char dbgCamStrRndmTrgt[];
extern char dbgCamStrUperVec[];
extern char dbgCamStrPers[];

void VuVecNormalize(f32 *, f32 *);

void DbgCameraPage(void) {
    s32 v;
    s32 i;
    f32 k;

    dbgPad = &padPort1;
    dbgCamLine = DbgMenuLineSelect(dbgCamLine, 13);
    switch (dbgCamLine) {
    case 0:
        dbgCamCursor = 0;
        if (dbgPad->repeatFast & 0x8000) {
            dbgCamModeNo--;
        }
        if (dbgPad->repeatFast & 0x2000) {
            dbgCamModeNo++;
        }
        i = dbgCamModeNo;
        if (i >= 5) {
            dbgCamModeNo = 0;
            i = 0;
        }
        if (i < 0) {
            dbgCamModeNo = 4;
            i = 4;
        }
        dbgCamera->mode = dbgCamModes[i];
        break;
    case 1:
        dbgCamCursor = 1;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->eye[0] -= 10.0f;
        }
        if (v < -60) {
            dbgCamera->eye[0] += 10.0f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->eye[1] += 10.0f;
        }
        if (v < -60) {
            dbgCamera->eye[1] -= 10.0f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->eye[2] -= 10.0f;
        }
        if (v < -60) {
            dbgCamera->eye[2] += 10.0f;
        }
        break;
    case 2:
        dbgCamCursor = 2;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->lookAt[0] -= 10.0f;
        }
        if (v < -60) {
            dbgCamera->lookAt[0] += 10.0f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->lookAt[1] += 10.0f;
        }
        if (v < -60) {
            dbgCamera->lookAt[1] -= 10.0f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->lookAt[2] -= 10.0f;
        }
        if (v < -60) {
            dbgCamera->lookAt[2] += 10.0f;
        }
        break;
    case 3:
        dbgCamCursor = 3;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->dir[0] -= 0.01f;
        }
        if (v < -60) {
            dbgCamera->dir[0] += 0.01f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->dir[1] += 0.01f;
        }
        if (v < -60) {
            dbgCamera->dir[1] -= 0.01f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->dir[2] -= 0.01f;
        }
        if (v < -60) {
            dbgCamera->dir[2] += 0.01f;
        }
        VuVecNormalize(dbgCamera->dir, dbgCamera->dir);
        break;
    case 4:
        dbgCamCursor = 4;
        k = 0.00087266444f;
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->pitch += v * k;
        }
        if (v < -60) {
            dbgCamera->pitch += v * k;
        }
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->yaw += v * k;
        }
        if (v < -60) {
            dbgCamera->yaw += v * k;
        }
        break;
    case 5:
        dbgCamCursor = 5;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->dist += 10.0f;
        }
        if (v < -60) {
            dbgCamera->dist -= 10.0f;
        }
        break;
    case 6:
        dbgCamCursor = 6;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->zoom += 0.1f;
        }
        if (v < -60) {
            dbgCamera->zoom -= 0.1f;
        }
        if (dbgCamera->zoom < 0.1f) {
            dbgCamera->zoom = 0.1f;
        }
        break;
    case 7:
        dbgCamCursor = 7;
        if (dbgPad->repeatFast & 0x20) {
            dbgCamera->shakeMode = 0x18;
            dbgCamera->shakeTick = 0;
            dbgCamera->shakeFlags |= 1;
        }
        if (dbgPad->repeatFast & 0x8000) {
            dbgCamera->shakeTime -= 60;
        }
        if (dbgPad->repeatFast & 0x2000) {
            dbgCamera->shakeTime += 60;
        }
        break;
    case 8:
        dbgCamCursor = 8;
        if (dbgPad->repeatFast & 0x8000) {
            dbgCamera->shakePeriod -= 60;
        }
        if (dbgPad->repeatFast & 0x2000) {
            dbgCamera->shakePeriod += 60;
        }
        break;
    case 9:
        dbgCamCursor = 9;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->shakeAmp[0] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeAmp[0] += 1.0f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->shakeAmp[1] += 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeAmp[1] -= 1.0f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->shakeAmp[2] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeAmp[2] += 1.0f;
        }
        break;
    case 10:
        dbgCamCursor = 10;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->shakeRand[0] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeRand[0] += 1.0f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->shakeRand[1] += 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeRand[1] -= 1.0f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->shakeRand[2] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeRand[2] += 1.0f;
        }
        break;
    case 11:
        dbgCamCursor = 11;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->shakeCycle[0] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeCycle[0] += 1.0f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->shakeCycle[1] += 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeCycle[1] -= 1.0f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->shakeCycle[2] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeCycle[2] += 1.0f;
        }
        break;
    case 12:
        dbgCamCursor = 12;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->shakeAmp2[0] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeAmp2[0] += 1.0f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->shakeAmp2[1] += 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeAmp2[1] -= 1.0f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->shakeAmp2[2] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeAmp2[2] += 1.0f;
        }
        break;
    case 13:
        dbgCamCursor = 13;
        v = dbgPad->leftX - 0x80;
        if (v > 60) {
            dbgCamera->shakeRand2[0] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeRand2[0] += 1.0f;
        }
        v = dbgPad->leftY - 0x80;
        if (v > 60) {
            dbgCamera->shakeRand2[1] += 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeRand2[1] -= 1.0f;
        }
        v = dbgPad->rightY - 0x80;
        if (v > 60) {
            dbgCamera->shakeRand2[2] -= 1.0f;
        }
        if (v < -60) {
            dbgCamera->shakeRand2[2] += 1.0f;
        }
        break;
    }
    DbgPuts(2, 3, dbgCamStrCameraMenu);
    DbgMenuCursorDraw(2, dbgCamCursor + 5);
    DbgPrintf(3, 5, dbgCamStrMode, dbgCamModeNo, dbgCamModeNames[dbgCamModeNo]);
    DbgPrintf(3, 6, dbgCamStrPos, dbgCamera->eye[0], dbgCamera->eye[1], dbgCamera->eye[2]);
    DbgPrintf(3, 7, dbgCamStrTarget, dbgCamera->lookAt[0], dbgCamera->lookAt[1], dbgCamera->lookAt[2]);
    DbgPrintf(3, 8, dbgCamStrEyeVec, dbgCamera->dir[0], dbgCamera->dir[1], dbgCamera->dir[2]);
    DbgPrintf(3, 9, dbgCamStrAngle, dbgCamera->pitch * 57.295776f, dbgCamera->yaw * 57.295776f, dbgCamera->roll * 57.295776f);
    DbgPrintf(3, 10, dbgCamStrDst, dbgCamera->dist);
    DbgPrintf(3, 11, dbgCamStrZoom, dbgCamera->zoom);
    DbgPrintf(3, 12, dbgCamStrQuakeTime, dbgCamera->shakeTime);
    DbgPrintf(3, 13, dbgCamStrNextCycl, dbgCamera->shakePeriod);
    DbgPrintf(3, 14, dbgCamStrBase, dbgCamera->shakeAmp[0], dbgCamera->shakeAmp[1], dbgCamera->shakeAmp[2]);
    DbgPrintf(3, 15, dbgCamStrRndm, dbgCamera->shakeRand[0], dbgCamera->shakeRand[1], dbgCamera->shakeRand[2]);
    DbgPrintf(3, 16, dbgCamStrCycl, dbgCamera->shakeCycle[0], dbgCamera->shakeCycle[1], dbgCamera->shakeCycle[2]);
    DbgPrintf(3, 17, dbgCamStrBaseTrgt, dbgCamera->shakeAmp2[0], dbgCamera->shakeAmp2[1], dbgCamera->shakeAmp2[2]);
    DbgPrintf(3, 18, dbgCamStrRndmTrgt, dbgCamera->shakeRand2[0], dbgCamera->shakeRand2[1], dbgCamera->shakeRand2[2]);
    DbgPrintf(3, 19, dbgCamStrUperVec, dbgCamera->up[0], dbgCamera->up[1], dbgCamera->up[2]);
    DbgPrintf(3, 20, dbgCamStrPers, dbgCamera->fov);
}
