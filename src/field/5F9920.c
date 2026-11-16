#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

void VuVecCopy(f32 *, f32 *);
void ChrTrailPush(struct ChrTrail *, f32 *, s32);

void ChrFollowersSnapToLeader(void) {
    FVECTOR v;
    ChrObj *pl;
    ChrObj *c;
    s32 i;

    pl = ChrGetByNo(1);
    VuVecCopy(v, pl->model->pos);
    for (i = 0; i < 15; i++) {
        ChrTrailPush(pl->trail, v, 0);
    }
    for (i = 1; i < 9; i++) {
        c = ChrGetByNo(i + 1);
        if (c != NULL && EvtSystemFlagGet(i + 0x1F) == 1) {
            VuVecCopy(c->model->pos, v);
            c->model->rot[0] = pl->model->rot[0];
            c->model->rot[1] = pl->model->rot[1];
            c->model->rot[2] = pl->model->rot[2];
            c->heading = pl->heading;
            c->moveDir = pl->moveDir;
        }
    }
}
