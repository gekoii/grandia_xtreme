/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "camera.h"
#include "battle/camera.h"
#include "battle/misc.h"


void VuVecCopy(void *, void *);
void VuMtxStackReset(void);
void VuMtxStackLoad(Camera *);
void VuSinCosAngle(f32 *, f32 *, s32);

void BtlCamUpdate(void) {
    BtlCamCtrl *c = &btlCamCtrl;
    BtlCamParam *t;
    Camera *f;
    Camera *g;
    s32 s;
    s32 a, b, d;
    s32 a2, b2, d2;
    f32 sc[2];

    if (btlSlowWait <= 0 && sysState.pauseMode != 2) {
        if ((sysState.dbgMenuFlags & 0x20000000) || (btlDebugCfg.flags & 8)) {
            if (--btlCamCtrl.frames <= 0) {
                c->duration = 0;
            }
            if (!(sysState.dbgMenuFlags & 0x0FFFFFFF)) {
                btlDebugCfg.flags &= 0xFFFFFFF7;
            } else {
                btlDebugCfg.flags |= 8;
            }
        } else {
            t = &btlCamParam;
            c->proc(c, t);
            BtlCamBlend();
            VuVecCopy(&btlCamera->lookAt, &t->target);
            VuVecCopy(&btlCamera->eye, &t->eye);
            a = 0x4000 - t->rot[0];
            b = -t->rot[1];
            d = -t->rot[2];
            g = btlCamera;
            g->pitch = (u16)a * 9.587378008291125e-05f;
            g->yaw = (u16)b * 9.587378008291125e-05f;
            g->roll = (u16)d * 9.587378008291125e-05f;
            g->dist = t->dist;
            g->fov = t->fov;
        }
        if (*(u8 *)&btlCamQuakeWait == 0) { /* u8 read (lbu): the s8 one changes the allocation */
            CamUpdateShake(btlCamera);
            btlCamQuakeWait = (u8)btlCamQuakeInterval;
        } else {
            btlCamQuakeWait--;
        }
    }
    VuMtxStackReset();
    CamApply(btlCamera);
    VuVecCopy(&btlCamParam.target, &btlCamera->lookAt);
    VuVecCopy(&btlCamParam.eye, &btlCamera->eye);
    f = btlCamera;
    a2 = f->pitch * 10430.380859375f;
    b2 = f->yaw * 10430.380859375f;
    d2 = f->roll * 10430.380859375f;
    btlCamParam.dist = f->dist;
    btlCamParam.rot[0] = (0x4000 - a2) & 0xFFFF;
    btlCamParam.rot[1] = -b2 & 0xFFFF;
    btlCamParam.rot[2] = -d2 & 0xFFFF;
    if (f->eye[1] < 5.0f && !(btlCamParam.flags & 1) && !(sysState.dbgMenuFlags & 0x20000000) && !(btlDebugCfg.flags & 8)) {
        s = f->mode;
        BtlCamSetCalcMode(2);
        btlCamera->eye[1] = 5.0f;
        CamApply(btlCamera);
        btlCamera->mode = s;
    }
    VuMtxStackLoad(btlCamera);
    VuMtxSetRotXY(sprBillboardMatrix, btlCamera->pitch, btlCamera->yaw, btlCamera->roll);
    VuSinCosAngle(&sc[0], &sc[1], btlCamera->fov * 91.02222442626953f);
    sprFocalLength = sc[1] / sc[0] * 320.0f;
    EnvFogUpdate();
    SndListenerSetFromCamera(btlCamera);
}
