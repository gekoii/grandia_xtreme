#include "common.h"
#include "battle/camera.h"
#include "party.h"

void BtlCamProcUnitFollow(BtlCamCtrl *cam, BtlCamParam *task) {
    if (task->phase == 0) {
        cam->to.rot[0] = 0x3600;
        cam->to.rot[2] = 0;
        task->phase++;
        cam->to.target[1] = task->unitPtr->pos[1] + task->unitPtr->params->colH * 0.7f;
        cam->to.dist = 800.0f;
        cam->to.fov = 40.0f;
    }
    cam->to.target[0] = task->unitPtr->pos[0];
    cam->to.target[2] = task->unitPtr->pos[2];
}
