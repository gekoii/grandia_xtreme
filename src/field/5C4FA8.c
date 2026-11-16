/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"
#include "field/state.h"


s32 ChrTenantIsInLeaderView(ChrObj *obj, f32 extra, f32 view) {
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
    fov = view * 0.5f;
    range = extra + obj->radius;
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
    a = FldCalcYaw(obj->model->pos, pl->model->pos) * 0.0054931641f;
    b = camField[0].yaw * 57.295776f;
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
