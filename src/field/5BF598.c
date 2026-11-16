#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccChrFaceForwardStart(u8 *arg0) {
    OccChrFace *a;
    ChrObj *o = NULL;

    a = OCC_PARAMS(arg0, OccChrFace);
    switch (a->target) {
    case 0:
        o = ChrGetById(a->id);
        break;
    case 1:
        o = ChrTenantGet(a->id);
        break;
    }
    if (o != NULL) {
        o->faceAnim.face = a->face;
        if (a->noBlink != 0) {
            o->faceAnim.mode = 0;
        } else {
            o->faceAnim.mode = 1;
        }
    }
}
