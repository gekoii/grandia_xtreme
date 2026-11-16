/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


s32 ChrTenantCanSeeLeader(ChrObj *obj, ChrEnemyWork *sense) {
    ChrObj *pl;
    f32 range;
    f32 fov;
    f32 d;
    f32 a;
    f32 b;

    pl = ChrGetByNo(1);
    if (pl == NULL) {
        return 0;
    }
    if (obj->sense == NULL) {
        range = obj->radius + 1200.0f;
        fov = 90.0f;
    } else {
        if (!(CHR_SENSE(obj)->flags & 4)) {
            return 0;
        }
        if (CHR_SENSE(obj)->range == 0) {
            return 0;
        }
        if (CHR_SENSE(obj)->fov == 0) {
            return 0;
        }
        range = CHR_SENSE(obj)->range;
        fov = CHR_SENSE(obj)->fov / 2;
    }
    if (!(obj->flags & 1)) {
        return 0;
    }
    if (obj->flags & 0x40) {
        return 0;
    }
    obj->leaderDist = d = FldVecDistXZ(pl->model->pos, obj->model->pos);
    if (range < d) {
        return 0;
    }
    a = FldCalcYaw(pl->model->pos, obj->model->pos) * 0.0054931641f;
    b = obj->heading * 0.0054931641f;
    while (a > 360.0f) {
        a -= 360.0f;
    }
    while (a < -360.0f) {
        a += 360.0f;
    }
    while (b > 360.0f) {
        b -= 360.0f;
    }
    while (b < -360.0f) {
        b += 360.0f;
    }
    if (__builtin_fabsf(ChrAngleDiffDeg(b, a)) < fov) {
        return 1;
    }
    return 0;
}
