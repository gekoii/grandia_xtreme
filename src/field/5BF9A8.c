#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccChrLookForward(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccChrLook *a;
    ChrObj *o;

    obj->timer = 0;
    a = OCC_PARAMS(arg0, OccChrLook);
    switch (a->target) {
    case 0:
        o = ChrGetById(a->id);
        break;
    case 1:
        o = ChrTenantGet(a->id);
        break;
    case 2:
        o = ChrGetByPartySlot(a->id);
        break;
    default:
        o = NULL;
        break;
    }
    if (o != NULL && (o->actFlags & 0x1000)) {
        obj->timer = 1;
    }
}
