#include "common.h"
#include "pad.h"
#include "gph.h"
#include "field/state.h"

void VuVecCopy(void *, void *);

void CamAiFF(Camera *o, ChrObj *s, Pad *pad) {
    o->mode = CAM_MODE_FIXED;
    VuVecCopy(o->lookAt, s->model->pos);
    o->lookAt[1] += camCtl.height;
}
