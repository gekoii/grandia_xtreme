#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

void MemMove128(void *, void *, s32);
void VuVecCopy(void *, void *);

void CamSetEyeMode(u8 arg0) {
    ChrObj *c;
    f32 t;

    c = ChrGetByNo(1);
    if (arg0) {
        if (camCtl.aiType == CAM_AI_EYE) {
            MemMove128(&camField[0], &camCtl.saved, 0x18);
            camCtl.aiType = camCtl.prevType;
            VuVecCopy(camField[0].lookAt, c->model->pos);
            t = 2.0f * (camCtl.target->model->cullSphere[3] - 20.0f);
            camField[0].pitch = 0.13962634f;
            camField[0].dist = 800.0f;
            camField[0].yaw = c->heading * 0.0000958738f;
            camCtl.height = t;
            camField[0].lookAt[1] += t;
            CamResetToPlayer();
            FldSndSePlay(3, 0x7FE, 0);
        }
    } else if (camCtl.aiType != CAM_AI_FF && camCtl.prevType != CAM_AI_TOWN) {
        FldSndSePlay(3, 0x7FE, 0);
        MemMove128(&camCtl.saved, &camField[0], 0x18);
        camField[0].pitch = 0.0f;
        camField[0].yaw = c->heading * 0.0000958738f;
        camCtl.aiType = CAM_AI_EYE;
    }
}
