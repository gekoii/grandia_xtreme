#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamScrSetTurn(Camera *arg0, s32 arg1, s32 arg2) {
    f32 v;

    v = CamScrArgFloat(arg2);
    if (arg2 == CAM_SCR_KEEP2) {
        return;
    }
    if (arg2 == CAM_SCR_SAVESTART) {
        switch (arg1) {
        case 0:
            arg0->pitch = camScrSaveStartTurnX;
            break;
        case 1:
            arg0->yaw = camScrSaveStartTurnY;
            break;
        case 2:
            arg0->roll = camScrSaveStartTurnZ;
            break;
        }
    } else if (arg2 == CAM_SCR_SAVEMAP) {
        switch (arg1) {
        case 0:
            arg0->pitch = camScrSaveMapTurnX;
            break;
        case 1:
            arg0->yaw = camScrSaveMapTurnY;
            break;
        case 2:
            arg0->roll = camScrSaveMapTurnZ;
            break;
        }
    } else {
        switch (arg1) {
        case 0:
            arg0->pitch = v * 0.017453292f;
            break;
        case 1:
            arg0->yaw = v * 0.017453292f;
            break;
        case 2:
            arg0->roll = v * 0.017453292f;
            break;
        }
    }
}
