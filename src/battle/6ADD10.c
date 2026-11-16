#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "camera.h"
#include "battle/camera.h"
#include "battle/env.h"
#include "battle/misc.h"
#include "battle/n31.h"

void VuVecCopy(void *, void *);

void BtlCamInit(void) {
    EnvParam *inf;
    Camera *f;
    Camera *g;
    BtlCamParam *t;
    s32 a, b, c;

    btlCamera = &btlCameraWork;
    CamInit(&btlCameraWork, 0);
    t = &btlCamParam;
    btlCamera->clipFar = 50000.0f;
    inf = EnvParamGet(btlEntryMapNo);
    btlCamera->fogColor[0] = inf->fogColor[0];
    btlCamera->fogColor[1] = inf->fogColor[1];
    btlCamera->fogColor[2] = inf->fogColor[2];
    f = btlCamera;
    f->fogDist = inf->fogDist;
    f->fogMax = inf->fogMax;
    f->fogMin = inf->fogMin;
    f->shakeAmp[0] = 0.0f;
    f->shakeAmp[1] = 0.0f;
    f->shakeAmp[2] = 0.0f;
    f->shakeAmp[3] = 0.0f;
    f->shakeRand[0] = 0.0f;
    f->shakeRand[1] = 0.0f;
    f->shakeRand[2] = 0.0f;
    f->shakeRand[3] = 0.0f;
    f->shakeFlags &= 0xFFFE;
    f->shakeTick = 0;
    btlCamQuakeInterval = btlCamQuakeWait = 0;
    t->target[0] = t->target[1] = t->target[2] = t->eye[0] = t->eye[1] = t->eye[2] = 0.0f;
    t->dist = 3000.0f;
    t->fov = 45.0f;
    t->rot[0] = t->rot[1] = t->rot[2] = 0;
    VuVecCopy(&f->lookAt, &t->target);
    VuVecCopy(&btlCamera->eye, &t->eye);
    a = 0x4000 - t->rot[0];
    b = -t->rot[1];
    c = -t->rot[2];
    g = btlCamera;
    g->pitch = (u16)a * 9.587378008291125e-05f;
    g->yaw = (u16)b * 9.587378008291125e-05f;
    g->roll = (u16)c * 9.587378008291125e-05f;
    g->dist = t->dist;
    g->fov = t->fov;
    BtlCamSetCalcMode(0);
    btlCamParam.mode = 0;
    BtlCamSetMode(1, 0, NULL, NULL);
    t->subMode = 0;
    t->unit = 0;
    t->unk4 = 0;
    if ((btlEncounterFlags & 0x10) && btlEncounterFlags != 0x17 && btlEncounterFlags != 0x18) {
        btlCamDefaultProc = BtlCamProcBossOverview;
        btlCamIntroProc = BtlCamProcBossOverviewInit;
        btlCamBossUnit = btlEnemyUnits;
    } else {
        btlCamDefaultProc = BtlCamProcOverview;
        btlCamIntroProc = BtlCamProcOverviewInit;
        btlCamBossUnit = NULL;
    }
}
