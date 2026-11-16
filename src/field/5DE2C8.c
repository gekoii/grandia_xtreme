#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

void DbgFldChrColDraw(s32 arg0) {
    s32 i;
    ChrObj *o;

    for (i = 0; i < ChrTenantGetUsed(); i++) {
        o = ChrTenantGet(i);
        if (arg0 != 0) {
            FldDebugDrawCylinder(o->model->pos, 0x8000FF00, o->unk38, 30.0f);
        } else {
            FldDebugDrawCylinder(o->model->pos, 0x80FFFF00, o->radius, 80.0f);
        }
    }
}
