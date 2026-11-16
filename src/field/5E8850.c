/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"


void ChrPartyUpdateMotions(void) {
    s32 i;
    ChrObj *p;

    for (i = 0; i < 9; i++) {
        p = ChrGetByNo(i + 1);
        if (p != 0 && (p->flags & 2) && (p->flags & 1) && p->model != 0) {
            FldDmgPopUpdate(&p->hpGauge);
            ChrMotBlendUpdate(p);
            if ((p->actFlags & 0x80) && (s8)p->mot == 15 && p->model->motionMode == 0 && p->model->motion[0].frame < 20.0f) {
                p->model->motion[0].frame = 20.0f;
            }
            ChrMotUpdate(p->model, p->motSpeed);
            ChrFaceAnimUpdate(&p->faceAnim, p->model);
        }
    }
}
