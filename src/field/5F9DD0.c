/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

s32 ChrMoveTowardPos(ChrObj *, f32 *, f32, f32);

s32 ChrTalkApproach(ChrObj *c) {
    ChrObj *pl;
    f32 d;
    s32 ret;

    ret = 0;
    pl = ChrGetByNo(1);
    d = FldVecDistXZ(pl->model->pos, c->model->pos);
    ChrPlayerTurnTo(c);
    if (d > 100.0f) {
        ret = 1;
        ChrMoveTowardPos(pl, c->model->pos, 10.0f, 1.0f);
    } else {
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(pl->vel) : "memory");
    }
    return ret;
}
