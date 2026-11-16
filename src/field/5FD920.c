#include "common.h"
#include "camera.h"
#include "field/n20.h"

void VuVecCopy(f32 *, f32 *);

void EvtCharSelCamSet(Camera *cam) {
    cam->mode = CAM_MODE_FIXED;
    VuVecCopy(cam->eye, evtCharSelWork->eye);
    VuVecCopy(cam->lookAt, evtCharSelWork->look);
    cam->lookAt[1] += 90.0f;
}
