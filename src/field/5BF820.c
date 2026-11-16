#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccChrLookForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccChrLook *a = OCC_PARAMS(obj, OccChrLook);
    ChrObj *o;
    ChrObj *t;

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
    if (o != NULL && ChrModelFindPart((s32)o->model, 0x280) != NULL) {
        if (a->toChr != 0) {
            switch (a->otherTarget) {
            case 0:
                t = ChrGetById(a->otherId);
                break;
            case 1:
                t = ChrTenantGet(a->otherId);
                break;
            case 2:
                t = ChrGetByPartySlot(a->otherId);
                break;
            default:
                return;
            }
            ChrHeadTurnStart(o, t, a->mode, a->angle, a->speed, a->unkC);
        } else {
            ChrHeadTurnStart(o, NULL, a->mode, a->angle, a->speed, a->unkC);
        }
        obj->timer = 1;
    }
}
