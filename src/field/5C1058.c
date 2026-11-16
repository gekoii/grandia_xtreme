#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n13.h"


s32 ChrTurnToPlayerStep(ChrObj *arg0) {
    ChrObj *o = ChrGetByNo(1);
    s32 t = FldCalcYaw(arg0->model->pos, o->model->pos);
    GphModel *p = arg0->model;
    s32 cur = p->rot[1];
    s32 tt;
    s32 d;

    if (t != cur) {
        tt = t;
        d = cur - t;
        if (t - cur < d) {
            tt = t - 0x8000;
        }
        t = tt;
        if (t < cur) {
            s32 v = cur - 0x800;
            p->rot[1] = v;
            if (v < t) {
                p->rot[1] = t;
            }
            return 0;
        } else {
            s32 v = cur + 0x800;
            p->rot[1] = v;
            if (v < t) {
                p->rot[1] = t;
            }
            return 0;
        }
    }
    return 0;
}
