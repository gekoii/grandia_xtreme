#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"


s32 ChrTenantOpWalkToLeaderFlat(s32 arg0, ChrObj *obj, s32 arg2, s32 arg3) {
    GphModel *m;

    if (obj->opTimer > 0) {
        obj->opTimer--;
    } else if (obj->opState == 0xB) {
        obj->opState = 0;
        return 0;
    } else {
        obj->opState = 0xB;
        obj->opTimer = arg3;
    }
    m = ChrGetByNo(1)->model;
    obj->moveTarget[0] = m->pos[0];
    obj->moveTarget[1] = obj->model->pos[1];
    obj->headingDeg = obj->heading * 0.0054931641f;
    obj->moveTarget[2] = m->pos[2];
    return ChrTenantMoveToward(obj, obj->moveTarget, arg2, 1);
}
