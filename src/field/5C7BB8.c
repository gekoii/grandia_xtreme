/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"


void ChrMotBlendUpdate(ChrObj *o) {
    GphModel *a;

    if (o != NULL) {
        a = o->model;
        if (a != NULL && o->motTable != NULL && a->motionMode != 0) {
            a->blend += 1.0f / fldState.motionSmooth;
            if (a->blend >= 1.0f) {
                a->motion[0].frame = 0.0f;
                a->motion[0].motion = a->motion[1].motion;
                a->motionMode = 0;
                a->blend = 0.0f;
                a->motion[1].motion = NULL;
                ((s32 (*)(GphModel *))ChrModelClearDeformBufs)(a);
            }
        }
    }
}
