/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"
#include "battle/n31.h"
#include "protos/battle.h"

void VuVecNormalize(f32 *, f32 *);

void BtlCamProcBossOverview(BtlCamCtrl *cam, BtlCamParam *task) {
    f32 z;
    f32 x;
    f32 dx, dz;
    s32 t;

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
        t = rand();
        cam->ofs.target[0] = cam->ofs.target[2] = cam->ofs.dist = 0.0f;
        cam->to.rot[2] = (t >> 15) * (1.0f / 65536.0f) * 1024.0f + 65024.0f;
        task->count = 0;
        task->phase++;
    } else {
        t = ++task->count;
        if (t >= 60) {
            dx = cam->to.eye[0] - btlCamBossUnit->pos[0];
            dz = cam->to.eye[2] - btlCamBossUnit->pos[2];
            if (dx * dx + dz * dz < 640000.0f || t >= 2700) {
                task->phase = 0;
            }
        }
    }
    BtlCamCalcUnitsCentre(&x, &z, 1);
    x -= task->target[0];
    z -= task->target[2];
    cam->ofs.target[0] = (cam->ofs.target[0] + (x - cam->ofs.target[0]) * 0.06f) * 0.25f;
    cam->ofs.target[2] = (cam->ofs.target[2] + (z - cam->ofs.target[2]) * 0.06f) * 0.25f;
    cam->to.target[0] = task->target[0] + cam->ofs.target[0];
    cam->to.target[2] = task->target[2] + cam->ofs.target[2];
}
