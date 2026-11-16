#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n13.h"

void VuVecCopy(void *, void *);

void MapEntrancePlacePlayer(s32 id) {
    ChrObj *o;
    MapEntrance *it;
    f32 v[4];
    f32 rot[4];

    mapEntranceCur = id;
    o = ChrGetByNo(1);
    /* s32 cast: the id is passed unmasked (the definition takes u16) */
    it = ((MapEntrance *(*)(s32))MapEntranceFindById)(id);
    if (it == NULL) {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
        VuVecCopy(o->model->pos, v);
        return;
    }
    VuVecCopy(o->model->pos, it);
    if (it->keepYaw == 0) {
        camField[0].yaw = it->yaw * 0.017453292f;
    } else {
        camField[0].yaw = mapEntranceKeepYaw;
    }
    if (it->effect >= 0) {
        switch (it->effect) {
        case 1:
            GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, it->effectTime);
            break;
        case 2:
            /* cast: EABI argument order (float first) and s32 effect */
            ((void (*)(void *, f32, s32, s32, s32))GphScreenFxSet)(fldScreenFx, 0.0f, 4, 1, it->effectTime);
            SysScreenCaptureRequest();
            break;
        }
        if (it->walkTime > 0) {
            FldObjMoveVelocity(o, it->dir, 0, it->walkTime);
        } else {
            FldDirToRot(it->dir, rot);
            o->heading = (u16)(s32)(rot[1] * 10430.378f);
        }
    } else {
        GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, 6.0f);
        switch (it->effect) {
        case -1:
            FldObjMoveVelocity(o, it->dir, 0, 20.0f);
            break;
        case -2:
            FldDirToRot(it->dir, rot);
            o->heading = (u16)(s32)(rot[1] * 10430.378f);
            break;
        case -3:
            FldObjMoveVelocity(o, it->dir, 0, 40.0f);
            break;
        }
    }
}
