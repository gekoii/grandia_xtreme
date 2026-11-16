#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamScrPersInterpStart(CamScrInterp *arg0, CamScrInterpCmd *arg1, Camera *arg2) {
    f32 v;

    if (arg1->from == CAM_SCR_KEEP) {
        arg0->start = arg2->fov;
    } else {
        arg0->start = CamScrArgFloat(arg1->from);
    }
    v = CamScrArgFloat(arg1->to);
    arg0->total = arg1->total;
    arg0->end = v;
    arg0->period = arg1->period;
    arg0->delta = arg0->end - arg0->start;
    arg0->mode = arg1->mode;
    arg0->active = 1;
    arg0->timer = 1;
    arg0->count = 0;
    arg0->cur = 0.0f;
}
