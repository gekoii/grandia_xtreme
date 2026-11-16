/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"
#include "protos/battle.h"

void BtlCamProcOverviewInit(BtlCamCtrl *cam, BtlCamParam *task) {
    f32 z;
    f32 x;
    s32 a;

    if (task->phase == 0) {
        task->phase = 1;
        BtlCamCalcUnitsCentre(&x, &z, 0);
        cam->to.dist = BtlCamFarthestUnitDist(NULL, x, z) * 1.04f + 280.0f;
        a = 0x2F00 - (s32)(z / 150.0f * 2176.0f);
        if (a < 0x2600) {
            a = 0x2600;
        } else if (a > 0x3A00) {
            a = 0x3A00;
        }
        cam->to.rot[0] = a;
        a = 0x8000 - (s32)(x / 150.0f * 4608.0f);
        if (a < 0x6800) {
            a = 0x6800;
        } else if (a > 0x9800) {
            a = 0x9800;
        }
        cam->to.rot[1] = a;
        cam->to.target[1] = 100.0f;
        cam->to.rot[2] = 0;
        cam->to.target[0] = x = x * 0.8f;
        cam->to.target[2] = z = z * 0.9f + 100.0f;
    }
}
