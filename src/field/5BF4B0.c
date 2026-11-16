#include "common.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"

void VuVecCopy(void *, void *);

void OccCameraSetPosForwardStart(u8 *arg0) {
    CamCtl *s = &camCtl;
    OccCameraSetPos *a;

    if (s->unk8 >= 4) {
        a = OCC_PARAMS(arg0, OccCameraSetPos);
        camCtl.aiType = CAM_AI_FF;
        s->unk8 = 0;
        VuVecCopy(camField[0].eye, a);
        s->unk14 = a->unk1C;
        camField[0].fov = a->fov;
    }
}
