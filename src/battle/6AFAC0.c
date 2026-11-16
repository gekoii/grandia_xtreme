/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/camera.h"
#include "protos/battle.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

void BtlCamProcChaseUnit(BtlCamCtrl *cam, BtlCamParam *task) {
    BtlUnit *c;
    BtlUnit *c2;
    BtlUnit *c3;
    f32 dx, dz, dx2;
    f32 z0;

    if (task->phase < 2) {
        if (task->phase == 0) {
            BtlCamSetCalcMode(2);
            cam->to.eye[1] = 200.0f;
            z0 = 0.0f;
            btlCamChaseVelX = btlCamChaseVelZ = z0;
            btlCamChaseOfsX = z0;
            task->phase++;
        }
        c = &btlUnits[btlCamParam.unit];
        cam->to.target[0] = c->pos[0];
        cam->to.target[1] = c->pos[1] + 100.0f;
        cam->to.target[2] = c->pos[2];
        cam->to.eye[0] = c->pos[0] * 0.7f;
        cam->to.eye[2] = c->pos[2] + 800.0f;
        if (cam->duration == 0) {
            task->phase++;
        }
    } else {
        c2 = &btlUnits[btlCamParam.unit];
        dx = c2->pos[0] * 0.7f - cam->to.eye[0];
        dz = c2->pos[2] + 800.0f - cam->to.eye[2];
        if (ABS(dx) < 100.0f) {
            if (btlCamChaseVelX > 0.0f) {
                btlCamChaseVelX -= 0.05f;
                if (btlCamChaseVelX < 0.0f) {
                    btlCamChaseVelX = 0.0f;
                }
            } else {
                btlCamChaseVelX += 0.05f;
                if (btlCamChaseVelX > 0.0f) {
                    btlCamChaseVelX = 0.0f;
                }
            }
        } else if (dx > 0.0f) {
            btlCamChaseVelX += 0.05f;
            if (btlCamChaseVelX > 2.0f) {
                btlCamChaseVelX = 2.0f;
            }
        } else {
            btlCamChaseVelX -= 0.05f;
            if (btlCamChaseVelX < -2.0f) {
                btlCamChaseVelX = -2.0f;
            }
        }
        if (ABS(dz) < 100.0f) {
            if (btlCamChaseVelZ > 0.0f) {
                btlCamChaseVelZ -= 0.05f;
                if (btlCamChaseVelZ < 0.0f) {
                    btlCamChaseVelZ = 0.0f;
                }
            } else {
                btlCamChaseVelZ += 0.05f;
                if (btlCamChaseVelZ > 0.0f) {
                    btlCamChaseVelZ = 0.0f;
                }
            }
        } else if (dz > 0.0f) {
            btlCamChaseVelZ += 0.05f;
            if (btlCamChaseVelZ > 4.0f) {
                btlCamChaseVelZ = 4.0f;
            }
        } else {
            btlCamChaseVelZ -= 0.05f;
            if (btlCamChaseVelZ < -4.0f) {
                btlCamChaseVelZ = -4.0f;
            }
        }
        cam->to.eye[0] += btlCamChaseVelX;
        cam->to.eye[2] += btlCamChaseVelZ;
        c3 = &btlUnits[btlCamParam.unit];
        dx2 = c3->pos[0] - cam->to.target[0];
        if (ABS(dx2) < 50.0f) {
            if (btlCamChaseOfsX > 0.0f) {
                btlCamChaseOfsX -= 1.0f;
                if (btlCamChaseOfsX < 0.0f) {
                    btlCamChaseOfsX = 0.0f;
                }
            } else {
                btlCamChaseOfsX += 1.0f;
                if (btlCamChaseOfsX > 0.0f) {
                    btlCamChaseOfsX = 0.0f;
                }
            }
        } else if (dx2 > 0.0f) {
            btlCamChaseOfsX += 6.0f;
            if (btlCamChaseOfsX > 150.0f) {
                btlCamChaseOfsX = 150.0f;
            }
        } else {
            btlCamChaseOfsX -= 6.0f;
            if (btlCamChaseOfsX < -150.0f) {
                btlCamChaseOfsX = -150.0f;
            }
        }
        c3 = &btlUnits[btlCamParam.unit];
        cam->to.target[0] += (dx2 + btlCamChaseOfsX) * 0.05f;
        cam->to.target[1] = c3->pos[1] + 100.0f;
        cam->to.target[2] += (c3->pos[2] - cam->to.target[2]) * 0.05f;
    }
    cam->to.rot[2] = btlCamChaseOfsX * 5.0f;
}
