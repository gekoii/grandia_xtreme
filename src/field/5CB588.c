#include "common.h"
#include "gph.h"
#include "pad.h"
#include "protos/field.h"
#include "field/state.h"

void VuVecCopy(void *, void *);

void CamAiGrandia2(Camera *o, ChrObj *s, Pad *pad) {
    o->mode = CAM_MODE_NORMAL;
    o->yaw = CamPadTurnGet(o, pad, 0, 0.93f, o->yaw * 57.295776f) * 0.017453292f;
    if (pad->rightY < 0x68) {
        o->pitch -= (f32)((0x68 - pad->rightY) * 2) * 9.58738e-05f;
    } else if (pad->rightY > 0x98) {
        o->pitch += (f32)((pad->rightY - 0x98) * 2) * 9.58738e-05f;
    }
    o->dist += (f32)(pad->pressure[1] >> 1);
    o->dist -= (f32)(pad->pressure[0] >> 1);
    VuVecCopy(o->lookAt, s->model->pos);
    o->lookAt[1] += camCtl.height;
}
