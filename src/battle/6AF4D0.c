#include "common.h"
#include "battle/camera.h"
#include "protos/battle.h"

void BtlCamProcUnitCloseUp(BtlCamCtrl *cam, BtlCamParam *task) {
    if (task->phase == 0) {
        cam->to.dist = (rand() >> 15) * (1.0f / 65536.0f) * 150.0f + 300.0f;
        cam->to.target[1] = BtlChrGetCenterY(task->unitPtr) + 30.0f;
        cam->to.rot[0] = (rand() >> 15) * (1.0f / 65536.0f) * 1536.0f + 14336.0f;
        cam->to.rot[2] = (rand() >> 15) * (1.0f / 65536.0f) * 1024.0f + 65024.0f;
        task->phase++;
    }
    cam->to.target[0] = task->unitPtr->pos[0];
    cam->to.target[2] = task->unitPtr->pos[2];
}
