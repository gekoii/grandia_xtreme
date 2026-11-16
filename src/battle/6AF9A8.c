#include "common.h"
#include "battle/camera.h"
#include "battle/n31.h"

void BtlCamProcBehindUnit(BtlCamCtrl *cam, BtlCamParam *task) {
    BtlUnit *o = &btlUnits[btlCamParam.unit];
    BtlUnit *o2;
    s32 ang;
    s32 d;

    if (task->phase == 0) {
        cam->to.rot[2] = 0;
        cam->to.rot[0] = 0x4000;
        cam->to.dist = 300.0f;
        task->phase++;
        cam->to.target[1] = 100.0f;
    }
    cam->to.target[0] = o->pos[0];
    ang = cam->to.rot[1];
    cam->to.target[2] = o->pos[2];
    o2 = &btlUnits[btlCamParam.unit];
    d = (o2->rot[1] - (ang + btlCamParam.unk4)) & 0xFFFF;
    if (d > 0x1000 && d < 0xF000) {
        if (d > 0x8000) {
            d -= 0xF000;
        } else {
            d -= 0x1000;
        }
        cam->ofs.rot[1] = (cam->ofs.rot[1] + (s32)((d - cam->ofs.rot[1]) * 0.05f)) / 4;
    } else {
        cam->ofs.rot[1] = 0;
    }
    cam->to.rot[1] = ang + cam->ofs.rot[1];
}
