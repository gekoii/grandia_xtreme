#include "common.h"
#include "gph.h"
#include "field/state.h"

s32 ChrTenantOpTurnTo(s32 arg0, ChrObj *obj, s32 target, s32 speed) {
    s32 d;

    if (obj->opTimer > 0) {
        obj->opTimer--;
        obj->heading = obj->moveDir = ((s32)(obj->headingDeg * 182.04445f) + obj->moveDir) & 0xFFFF;
        return 1;
    }
    if (obj->opState == 0x1C) {
        obj->flags |= 0x80;
        obj->opState = 0;
        obj->opTimer = 0;
        obj->model->rot[1] = (s32)((target - 180) * 182.04445f) & 0xFFFF;
        obj->heading = obj->moveDir = (s32)(target * 182.04445f) & 0xFFFF;
        return 0;
    }
    if (speed == 0) {
        obj->opState = 0;
        obj->opTimer = 0;
        obj->model->rot[1] = (s32)((target - 180) * 182.04445f) & 0xFFFF;
        obj->heading = obj->moveDir = (s32)(target * 182.04445f) & 0xFFFF;
        return 0;
    }
    obj->opState = 0x1C;
    obj->flags &= ~0x80;
    d = target - (s32)(obj->moveDir * 0.0054931641f);
    while (d > 360) d -= 360;
    while (d < -360) d += 360;
    if (d > 180) {
        d -= 360;
    } else if (d < -180) {
        d += 360;
    }
    obj->opTimer = __builtin_fabsf((f32)(d / speed));
    obj->heading = obj->moveDir = (obj->moveDir + (s32)((d % speed) * 182.04445f)) & 0xFFFF;
    if (d > 0) {
        obj->headingDeg = speed;
    } else {
        obj->headingDeg = -speed;
    }
    return 1;
}
