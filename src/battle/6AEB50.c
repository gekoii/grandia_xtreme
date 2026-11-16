/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"
#include "battle/n31.h"
#include "protos/battle.h"

void BtlCamProcOverview(BtlCamCtrl *cam, BtlCamParam *task) {
    f32 z;
    f32 x;
    s32 a;
    s32 b;

    if (task->phase == 0) {
        cam->ofs.rot[1] = 0;
        cam->ofs.rot[0] = 0;
        task->phase++;
        cam->ofs.target[0] = cam->ofs.target[2] = cam->ofs.dist = 0.0f;
        task->count = 0;
    }
    BtlCamCalcUnitsCentre(&x, &z, 0);
    cam->ofs.dist = (cam->ofs.dist + ((BtlCamFarthestUnitDist(NULL, x, z) * 1.04f + 280.0f - task->dist) - cam->ofs.dist) * 0.06f) * 0.25f;
    BtlCamCalcUnitsCentre(&x, &z, 1);
    a = 0x2F00 - (s32)(z / 150.0f * 2176.0f);
    if (a < 0x2600) {
        a = 0x2600;
    } else if (a > 0x3A00) {
        a = 0x3A00;
    }
    a -= (u16)task->rot[0];
    cam->ofs.rot[0] = (cam->ofs.rot[0] + (s32)((a - cam->ofs.rot[0]) * 0.06f)) / 4;
    b = 0x8000 - (s32)(x / 150.0f * 4608.0f);
    if (b < 0x6800) {
        b = 0x6800;
    } else if (b > 0x9800) {
        b = 0x9800;
    }
    b -= (u16)task->rot[1];
    x = x * 0.8f - task->target[0];
    z = z * 0.9f + 100.0f - task->target[2];
    cam->ofs.target[0] = (cam->ofs.target[0] + (x - cam->ofs.target[0]) * 0.06f) * 0.25f;
    cam->ofs.target[2] = (cam->ofs.target[2] + (z - cam->ofs.target[2]) * 0.06f) * 0.25f;
    cam->ofs.rot[1] = (cam->ofs.rot[1] + (s32)((b - cam->ofs.rot[1]) * 0.06f)) / 4;
    cam->to.target[0] = task->target[0] + cam->ofs.target[0];
    cam->to.rot[0] = task->rot[0] + cam->ofs.rot[0];
    cam->to.target[2] = task->target[2] + cam->ofs.target[2];
    cam->to.rot[1] = task->rot[1] + cam->ofs.rot[1];
    cam->to.dist = task->dist + cam->ofs.dist;
    cam->to.rot[2] = 0;
    cam->to.target[1] = 100.0f;
}
