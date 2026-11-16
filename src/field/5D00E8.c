#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"

void CamScrSetMove(Camera *arg0, s32 arg1, s32 arg2) {
    f32 v;
    f32 vec[3];
    GphModel *o;

    v = CamScrArgFloat(arg2);
    if (arg2 == CAM_SCR_KEEP) {
        return;
    }
    if (arg2 == CAM_SCR_PLAYER) {
        o = camCtl.target->model;
        vec[0] = o->pos[0];
        vec[1] = o->pos[1] + camCtl.height;
        vec[2] = o->pos[2];
        switch (arg1) {
        case 0:
            arg0->lookAt[0] = vec[0];
            break;
        case 1:
            arg0->lookAt[1] = vec[1];
            break;
        case 2:
            arg0->lookAt[2] = vec[2];
            break;
        }
    } else {
        switch (arg1) {
        case 0:
            arg0->lookAt[0] = v;
            break;
        case 1:
            arg0->lookAt[1] = v;
            break;
        case 2:
            arg0->lookAt[2] = v;
            break;
        }
    }
}
