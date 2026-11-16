#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccChrTurnForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccChrTurn *a = OCC_PARAMS(obj, OccChrTurn);
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
    if (o != NULL) {
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
            ChrTurnStart(o, t, a->mode, 0, a->speed, a->unk10);
        } else {
            ChrTurnStart(o, NULL, a->mode, a->angle, a->speed, a->unk10);
        }
        obj->timer = 1;
    }
}
