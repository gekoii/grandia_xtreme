/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamScrTurnInterpStart(CamScrInterp *arg0, s32 arg1, CamScrInterpCmd *arg2, Camera *arg3) {
    f32 v;

    if (arg2->from == CAM_SCR_KEEP) {
        switch (arg1) {
        case 0:
            arg0->start = arg3->pitch * 57.295776f;
            break;
        case 1:
            arg0->start = arg3->yaw * 57.295776f;
            v = arg0->start;
            arg0->start = CamAngleWrap180(v);
            break;
        case 2:
            arg0->start = arg3->roll * 57.295776f;
            break;
        }
    } else {
        v = CamScrArgFloat(arg2->from);
        arg0->start = v;
        if (arg1 == 1) {
            arg0->start = CamAngleWrap180(v);
        }
    }
    if (arg2->to == CAM_SCR_SAVESTART) {
        switch (arg1) {
        case 0:
            arg0->end = camScrSaveStartTurnX * 57.295776f;
            break;
        case 1:
            arg0->end = camScrSaveStartTurnY * 57.295776f;
            break;
        case 2:
            arg0->end = camScrSaveStartTurnZ * 57.295776f;
            break;
        }
    } else if (arg2->to == CAM_SCR_SAVEMAP) {
        switch (arg1) {
        case 0:
            arg0->end = camScrSaveMapTurnX * 57.295776f;
            break;
        case 1:
            arg0->end = camScrSaveMapTurnY * 57.295776f;
            break;
        case 2:
            arg0->end = camScrSaveMapTurnZ * 57.295776f;
            break;
        }
    } else {
        v = CamScrArgFloat(arg2->to);
        arg0->end = v;
        if (arg1 == 1) {
            arg0->end = CamAngleWrap180(v);
        }
    }
    arg0->total = arg2->total;
    arg0->period = arg2->period;
    arg0->mode = arg2->mode;
    switch (arg2->turnDir) {
    case 0:
        if (arg0->start <= arg0->end) {
            if (arg0->end - arg0->start > 180.0f) {
                arg0->end -= 360.0f;
            }
        } else {
            if (arg0->start - arg0->end >= 180.0f) {
                arg0->end += 360.0f;
            }
        }
        break;
    case 1:
        if (arg0->start > arg0->end) {
            arg0->end += 360.0f;
        }
        break;
    case 2:
        if (arg0->start < arg0->end) {
            arg0->end -= 360.0f;
        }
        break;
    }
    arg0->active = 1;
    arg0->timer = 1;
    arg0->delta = arg0->end - arg0->start;
    arg0->count = 0;
    arg0->cur = 0.0f;
}
