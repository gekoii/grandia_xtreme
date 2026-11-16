#include "common.h"
#include "battle/camera.h"
#include "battle/n31.h"

void BtlCamProcFar(BtlCamCtrl *cam, BtlCamParam *task) {
    cam->to.target[0] = 0.0f;
    cam->to.target[1] = 0.0f;
    cam->to.dist = 2500.0f;
    cam->to.target[2] = 100.0f;
    cam->to.rot[0] = 0x2800;
    cam->to.rot[1] = 0x8000;
    cam->to.rot[2] = 0;
}
