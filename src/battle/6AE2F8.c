#include "common.h"
#include "battle/camera.h"
#include "battle/n31.h"
#include "protos/battle.h"

void VuVecCopy(void *, void *);

void BtlCamSetMode(s32 mode, s32 arg1, BtlUnit *arg2, BtlCamProc proc) {
    BtlCamCtrl *cam = &btlCamCtrl;
    BtlCamParam *t = &btlCamParam;

    btlCamParam.mode = mode;
    t->flags = 0;
    t->phase = 0;
    cam->duration = arg1;
    t->unitPtr = arg2;
    cam->frames = arg1;
    VuVecCopy(&cam->from.target, &t->target);
    VuVecCopy(&cam->to.target, &t->target);
    VuVecCopy(&cam->from.eye, &t->eye);
    VuVecCopy(&cam->to.eye, &t->eye);
    cam->from.rot[0] = t->rot[0];
    cam->from.rot[1] = t->rot[1];
    cam->from.rot[2] = t->rot[2];
    cam->from.dist = t->dist;
    cam->to.rot[0] = t->rot[0];
    cam->to.rot[1] = t->rot[1];
    cam->to.rot[2] = t->rot[2];
    cam->to.dist = t->dist;
    cam->from.fov = t->fov;
    cam->to.fov = 45.0f;
    switch (mode) {
    case 0:
        cam->proc = proc;
        break;
    case 1:
        switch (btlCamParam.subMode) {
        case 1:
            cam->proc = BtlCamProcFar;
            break;
        case 2:
            cam->proc = BtlCamProcChaseUnit;
            break;
        case 3:
            cam->proc = BtlCamProcBehindUnit;
            break;
        default:
            cam->proc = btlCamDefaultProc;
            break;
        }
        break;
    case 2:
        cam->proc = BtlCamProcUnitFixed;
        break;
    case 9:
        cam->proc = BtlCamProcUnitFollow;
        break;
    case 3:
        cam->proc = BtlCamProcUnitAndTarget;
        break;
    case 4:
        cam->proc = BtlCamProcUnitCloseUp;
        break;
    case 5:
        cam->proc = BtlCamProcUnitPair;
        break;
    case 6:
        cam->proc = BtlCamProcFar;
        break;
    case 7:
        cam->proc = btlCamIntroProc;
        break;
    case 8:
        cam->proc = BtlCamProcTargets;
        break;
    }
    BtlCamSetCalcMode(0);
}
