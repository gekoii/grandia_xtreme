#include "common.h"
#include "battle/camera.h"
#include "protos/battle.h"

void VuVecNormalize(f32 *, f32 *);

void BtlCamProcBossOverviewInit(BtlCamCtrl *cam, BtlCamParam *task) {
    f32 p[2];

    if (task->phase == 0) {
        BtlCamSetCalcMode(2);
        cam->to.eye[0] = btlCamBossUnit->pos[0];
        cam->to.eye[1] = 0.0f;
        cam->to.eye[2] = btlCamBossUnit->pos[2];
        VuVecNormalize(cam->to.eye, cam->to.eye);
        cam->to.eye[1] = 100.0f;
        cam->to.target[1] = 200.0f;
        cam->to.eye[0] *= -1100.0f;
        cam->to.eye[2] *= -1100.0f;
        BtlCamCalcUnitsCentre(&p[0], &p[1], 1);
        cam->to.target[0] = p[0];
        cam->to.target[2] = p[1];
        cam->to.rot[2] = (rand() >> 15) * (1.0f / 65536.0f) * 1024.0f + 65024.0f;
        task->phase++;
    }
}
