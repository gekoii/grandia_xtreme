#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"

s32 CamScrStart(s32 arg0) {
    if (camScrState != CAM_SCR_IDLE) return 0;
    if (CamScrGet(arg0) == NULL) return 0;
    camScrState = CAM_SCR_START;
    camScrId = arg0;
    camField[0].mode = CAM_MODE_NORMAL;
    if (camCtl.playerShown == 0) {
        if (camCtl.target != NULL) {
            camCtl.target->model->flags &= 0xFFF7;
        }
        camCtl.playerShown = 1;
    }
    return 0;
}
