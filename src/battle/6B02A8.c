#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "pad.h"
#include "sys.h"

extern char dbgBtlCamTitleFmt[];
extern char dbgBtlCamTitleColorStr[];
extern char dbgBtlCamColor0Str[];
extern char dbgBtlCamDistFmt[];
extern char dbgBtlCamTargetFmt[];
extern char dbgBtlCamPosFmt[];
extern char dbgBtlCamRotFmt[];
extern char dbgBtlCamFovFmt[];
extern char dbgBtlCamModeFmt[];
extern char dbgBtlCamFlagFmt[];
extern char dbgBtlCamPhaseFmt[];
extern char dbgBtlCamCountFmt[];
extern char dbgBtlCamQuakeStr[];
extern char dbgBtlCamQuakeTypeFmt[];
extern char dbgBtlCamQuakeCoordFmt[];
extern char dbgBtlCamQuakeViewStr[];
extern char dbgBtlCamQuakeWorldStr[];
extern char dbgBtlCamQuakeChangeStr[];
extern char dbgBtlCamQuakeTimeStr[];
extern char dbgBtlCamQuakeNextFmt[];
extern char dbgBtlCamQuakeTimeFmt[];
extern char dbgBtlCamQuakeIntFmt[];
extern char dbgBtlCamQuakeCycleFmt[];
extern char dbgBtlCamYFmt[];
extern char dbgBtlCamZFmt[];
extern char dbgBtlCamQuakeBase1Fmt[];
extern char dbgBtlCamQuakeRnd1Fmt[];
extern char dbgBtlCamQuakeOfsFmt[];
extern char dbgBtlCamQuakeBase2Fmt[];
extern char dbgBtlCamQuakeRnd2Fmt[];

void DbgBtlCameraPage(void) {
    Pad *pad;
    Camera *p;
    f32 f;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlPageSelect(pad, 1);
        if (sysState.dbgPage == 1) {
            DbgBtlLineSelect(pad, 0x16);
            switch (sysState.dbgCursor) {
            case 0:
                if (pad->repeat & 0xA000) {
                    btlCamera->shakeFlags ^= 1;
                }
                break;
            case 1:
                if (pad->repeat & 0xA000) {
                    btlCamera->shakeMode ^= 2;
                }
                break;
            case 2:
                if (pad->repeat & 0xA000) {
                    btlCamera->shakeMode ^= 4;
                }
                break;
            case 3:
                if (pad->repeat & 0xA000) {
                    btlCamera->shakeMode ^= 8;
                }
                break;
            case 4:
                if (pad->repeat & 0xA000) {
                    btlCamera->shakeMode ^= 0x10;
                }
                break;
            case 5:
                f = DbgEditValue(0, 0, 3, 1.0f);
                p = btlCamera;
                p->shakePeriod = (u32)((u32)p->shakePeriod + f);
                break;
            case 6:
                f = DbgEditValue(0, 0, 3, 1.0f);
                p = btlCamera;
                p->shakeTime = (u32)((u32)p->shakeTime + f);
                break;
            case 7:
                btlCamQuakeInterval = (u8)btlCamQuakeInterval + DbgEditValue(0, 0, 0, 1.0f);
                break;
            case 8:
                btlCamera->shakeCycle[0] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 9:
                btlCamera->shakeCycle[1] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 10:
                btlCamera->shakeCycle[2] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 11:
                btlCamera->shakeAmp[0] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 12:
                btlCamera->shakeAmp[1] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 13:
                btlCamera->shakeAmp[2] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 14:
                btlCamera->shakeRand[0] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 15:
                btlCamera->shakeRand[1] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 16:
                btlCamera->shakeRand[2] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 17:
                btlCamera->shakeAmp2[0] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 18:
                btlCamera->shakeAmp2[1] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 19:
                btlCamera->shakeAmp2[2] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 20:
                btlCamera->shakeRand2[0] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 21:
                btlCamera->shakeRand2[1] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            case 22:
                btlCamera->shakeRand2[2] += DbgEditValue(2, -1, 3, 1.0f);
                break;
            }
        }
    }
    DbgPrintf(2, 2, dbgBtlCamTitleFmt, dbgBtlCamTitleColorStr, dbgBtlCamColor0Str, sysState.dbgPage + 1);
    switch (sysState.dbgPage) {
    case 0:
        DbgPrintf(2, 3, dbgBtlCamDistFmt, btlCamParam.dist);
        DbgPrintf(2, 4, dbgBtlCamTargetFmt, btlCamParam.target[0], btlCamParam.target[1], btlCamParam.target[2]);
        DbgPrintf(2, 5, dbgBtlCamPosFmt, btlCamParam.eye[0], btlCamParam.eye[1], btlCamParam.eye[2]);
        DbgPrintf(2, 6, dbgBtlCamRotFmt, btlCamParam.rot[0] & 0xFFFFF, btlCamParam.rot[1] & 0xFFFFF,
                      btlCamParam.rot[2] & 0xFFFFF);
        DbgPrintf(2, 7, dbgBtlCamFovFmt, btlCamParam.fov);
        DbgPrintf(2, 8, dbgBtlCamModeFmt, btlCamParam.mode);
        DbgPrintf(2, 9, dbgBtlCamFlagFmt, btlCamParam.flags);
        DbgPrintf(2, 10, dbgBtlCamPhaseFmt, btlCamParam.phase);
        DbgPrintf(2, 11, dbgBtlCamCountFmt, btlCamParam.count);
        break;
    case 1:
        if (sysState.dbgCursor < 17) {
            DbgDrawCursor(1, sysState.dbgCursor + 3);
        } else {
            DbgDrawCursor(0x15, sysState.dbgCursor - 3);
        }
        DbgBtlPrintOnOff(2, 3, dbgBtlCamQuakeStr, btlCamera->shakeFlags & 1);
        DbgPrintf(2, 4, dbgBtlCamQuakeTypeFmt, (btlCamera->shakeMode >> 1) & 1);
        DbgPrintf(2, 5, dbgBtlCamQuakeCoordFmt, (btlCamera->shakeMode & 4) ? dbgBtlCamQuakeViewStr : dbgBtlCamQuakeWorldStr);
        DbgBtlPrintOnOff(2, 6, dbgBtlCamQuakeChangeStr, btlCamera->shakeMode & 8);
        DbgBtlPrintOnOff(2, 7, dbgBtlCamQuakeTimeStr, btlCamera->shakeMode & 0x10);
        DbgPrintf(2, 8, dbgBtlCamQuakeNextFmt, (u32)btlCamera->shakePeriod);
        DbgPrintf(2, 9, dbgBtlCamQuakeTimeFmt, (u32)btlCamera->shakeTime);
        DbgPrintf(2, 10, dbgBtlCamQuakeIntFmt, (u8)btlCamQuakeInterval);
        DbgPrintf(2, 11, dbgBtlCamQuakeCycleFmt, btlCamera->shakeCycle[0]);
        DbgPrintf(7, 12, dbgBtlCamYFmt, btlCamera->shakeCycle[1]);
        DbgPrintf(7, 13, dbgBtlCamZFmt, btlCamera->shakeCycle[2]);
        DbgPrintf(2, 14, dbgBtlCamQuakeBase1Fmt, btlCamera->shakeAmp[0]);
        DbgPrintf(7, 15, dbgBtlCamYFmt, btlCamera->shakeAmp[1]);
        DbgPrintf(7, 16, dbgBtlCamZFmt, btlCamera->shakeAmp[2]);
        DbgPrintf(2, 17, dbgBtlCamQuakeRnd1Fmt, btlCamera->shakeRand[0]);
        DbgPrintf(7, 18, dbgBtlCamYFmt, btlCamera->shakeRand[1]);
        DbgPrintf(7, 19, dbgBtlCamZFmt, btlCamera->shakeRand[2]);
        DbgPrintf(0x16, 11, dbgBtlCamQuakeOfsFmt, btlCamera->shakeOfs[0]);
        DbgPrintf(0x1B, 12, dbgBtlCamYFmt, btlCamera->shakeOfs[1]);
        DbgPrintf(0x1B, 13, dbgBtlCamZFmt, btlCamera->shakeOfs[2]);
        DbgPrintf(0x16, 14, dbgBtlCamQuakeBase2Fmt, btlCamera->shakeAmp2[0]);
        DbgPrintf(0x1B, 15, dbgBtlCamYFmt, btlCamera->shakeAmp2[1]);
        DbgPrintf(0x1B, 16, dbgBtlCamZFmt, btlCamera->shakeAmp2[2]);
        DbgPrintf(0x16, 17, dbgBtlCamQuakeRnd2Fmt, btlCamera->shakeRand2[0]);
        DbgPrintf(0x1B, 18, dbgBtlCamYFmt, btlCamera->shakeRand2[1]);
        DbgPrintf(0x1B, 19, dbgBtlCamZFmt, btlCamera->shakeRand2[2]);
        break;
    }
}
