#include "common.h"
#include "battle/camera.h"

void BtlCamProcUnitFixed(BtlCamCtrl *cam, BtlCamParam *task) {
    BtlUnit *c;

    if (task->phase == 0) {
        c = task->unitPtr;
        cam->to.rot[0] = 0x3600;
        cam->to.target[0] = c->pos[0];
        cam->to.rot[2] = 0;
        task->phase++;
        cam->to.target[1] = c->pos[1] + 100.0f;
        cam->to.target[2] = c->pos[2];
        cam->to.dist = 800.0f;
        cam->to.fov = 40.0f;
    }
}
