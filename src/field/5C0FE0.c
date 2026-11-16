#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n13.h"


s32 ChrPlayerTurnTo(ChrObj *arg0) {
    ChrObj *o = ChrGetByNo(1);
    o->heading = FldCalcYaw(o->model->pos, arg0->model->pos) + 0x8000;
    o->model->rot[1] = FldCalcYaw(o->model->pos, arg0->model->pos);
    return 0;
}
