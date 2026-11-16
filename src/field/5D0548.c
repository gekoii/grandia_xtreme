#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"

void CamScrMoveInterpStart(CamScrInterp *arg0, s32 arg1, CamScrInterpCmd *arg2, Camera *arg3) {
    f32 vec[3];
    GphModel *o;

    if (arg2->from == CAM_SCR_KEEP) {
        switch (arg1) {
        case 0:
            arg0->start = arg3->lookAt[0];
            break;
        case 1:
            arg0->start = arg3->lookAt[1];
            break;
        case 2:
            arg0->start = arg3->lookAt[2];
            break;
        }
    } else {
        arg0->start = CamScrArgFloat(arg2->from);
    }
    if (arg2->to == CAM_SCR_PLAYER) {
        o = camCtl.target->model;
        vec[0] = o->pos[0];
        vec[1] = o->pos[1] + camCtl.height;
        vec[2] = o->pos[2];
        switch (arg1) {
        case 0:
            arg0->end = vec[0];
            break;
        case 1:
            arg0->end = vec[1];
            break;
        case 2:
            arg0->end = vec[2];
            break;
        }
    } else {
        arg0->end = CamScrArgFloat(arg2->to);
    }
    arg0->total = arg2->total;
    arg0->period = arg2->period;
    arg0->mode = arg2->mode;
    arg0->active = 1;
    arg0->delta = arg0->end - arg0->start;
    arg0->timer = 1;
    arg0->count = 0;
    arg0->cur = 0.0f;
}
