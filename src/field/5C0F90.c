#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n13.h"


s32 ChrTurnToPlayer(ChrObj *arg0) {
    ChrObj *o = ChrGetByNo(1);
    arg0->moveDir = FldCalcYaw(arg0->model->pos, o->model->pos) + 0x8000;
    return 0;
}
