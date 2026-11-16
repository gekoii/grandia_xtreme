#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"

void VuSinCosDeg(f32 *s, f32 *c, f32 ang);

s32 ChrTenantOpWalkByLeaderAngle(void *arg0, ChrObj *a, s32 r, s32 time) {
    VuVec3 v;
    f32 s;
    f32 c;
    s32 ang;
    ChrObj *pc;
    GphModel *o;

    pc = ChrGetByNo(1);
    if (a->opTimer > 0) {
        a->opTimer--;
    } else {
        if (a->opState == 7) {
            a->opState = 0;
            return 0;
        }
        a->opState = 7;
        a->opTimer = time;
        ang = FldCalcYaw(pc->model->pos, a->model->pos);
        if (a->heading < ang) {
            ang -= a->heading;
        } else {
            ang += a->heading;
        }
        if (ang > 0x4000) {
            ang = 0x4000;
        }
        if ((u32)ang < 0xFFFFC000) {
            ang = -0x4000;
        }
        a->headingDeg = ang;
    }
    VuSinCosDeg(&s, &c, -(a->headingDeg * 0.0054931640625f));
    v.x = s;
    v.z = c;
    o = a->model;
    a->moveTarget[0] = o->pos[0] + v.x * r;
    a->moveTarget[1] = pc->model->pos[1];
    a->moveTarget[2] = o->pos[2] + v.z * r;
    if (!(a->flags & 0x400)) {
        a->moveTarget[1] = o->pos[1];
    }
    if (!(a->flags & 4)) {
        a->moveTarget[1] = o->pos[1];
    }
    ChrTenantMoveToward(a, a->moveTarget, r, 1);
    return 1;
}
