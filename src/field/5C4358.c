#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

void VuVecCopy(f32 *, f32 *);

s32 ChrTenantOpGoToLeaderPos(s32 arg0, ChrObj *obj, s32 arg2, s32 arg3) {
    if (obj->opState != 6) {
        obj->opState = 6;
        VuVecCopy(obj->moveTarget, ChrGetByNo(1)->model->pos);
    }
    if (!(obj->flags & 0x400)) {
        obj->moveTarget[1] = obj->model->pos[1];
    }
    if (!(obj->flags & 4)) {
        obj->moveTarget[1] = obj->model->pos[1];
    }
    obj->headingDeg = obj->heading * 0.0054931641f;
    return ChrTenantMoveToward(obj, obj->moveTarget, arg3, 1);
}
