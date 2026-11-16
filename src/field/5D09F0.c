#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamScrZoomInterpStart(CamScrInterp *arg0, CamScrInterpCmd *arg1, Camera *arg2) {
    if (arg1->from == CAM_SCR_KEEP) {
        arg0->start = arg2->zoom;
    } else {
        arg0->start = CamScrArgFloat(arg1->from);
    }
    if (arg1->to == CAM_SCR_SAVESTART) {
        arg0->end = camScrSaveStartZoom;
    } else if (arg1->to == CAM_SCR_SAVEMAP) {
        arg0->end = camScrSaveMapZoom;
    } else {
        arg0->end = CamScrArgFloat(arg1->to);
    }
    arg0->total = arg1->total;
    arg0->period = arg1->period;
    arg0->mode = arg1->mode;
    arg0->active = 1;
    arg0->delta = arg0->end - arg0->start;
    arg0->timer = 1;
    arg0->count = 0;
    arg0->cur = 0.0f;
}
