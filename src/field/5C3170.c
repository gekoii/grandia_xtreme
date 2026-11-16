#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"

void VuSinCosDeg(f32 *s, f32 *c, f32 ang);

s32 ChrTenantOpWalkRandomDir(void *arg0, ChrObj *a, s32 r, s32 time) {
    VuVec3 v;
    f32 s;
    f32 c;
    GphModel *o;

    if (a->opTimer > 0) {
        a->opTimer--;
    } else {
        if (a->opState == 9) {
            a->opState = 0;
            return 0;
        }
        a->opState = 9;
        a->opTimer = time;
        a->headingDeg = a->heading + (VuRandInt(0x8000) - 0x4000);
    }
    VuSinCosDeg(&s, &c, -(a->headingDeg * 0.0054931640625f));
    v.x = s;
    v.z = c;
    o = a->model;
    a->moveTarget[0] = o->pos[0] + v.x * r;
    a->moveTarget[1] = o->pos[1];
    a->moveTarget[2] = o->pos[2] + v.z * r;
    ChrTenantMoveToward(a, a->moveTarget, r, 1);
    return 1;
}
