#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


s32 ChrTenantOpWalkToLeader(void *arg0, ChrObj *a, s32 r, s32 time) {
    GphModel *o;

    if (a->opTimer > 0) {
        a->opTimer--;
    } else {
        if (a->opState == 0xB) {
            a->opState = 0;
            return 0;
        }
        a->opState = 0xB;
        a->opTimer = time;
    }
    o = ChrGetByNo(1)->model;
    a->moveTarget[0] = o->pos[0];
    a->headingDeg = a->heading * 0.0054931640625f;
    a->moveTarget[1] = o->pos[1];
    a->moveTarget[2] = o->pos[2];
    return ChrTenantMoveToward(a, a->moveTarget, r, 1);
}
