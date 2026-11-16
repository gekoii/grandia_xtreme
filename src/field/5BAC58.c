/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"

void ChrMotApplyRootMove(ChrObj *);

void OccMotionCtrlForward(OccEntry *o) {
    OccMotionCtrl *s = OCC_PARAMS(o, OccMotionCtrl);
    ChrObj *e = NULL;
    GphModel *h;

    if (OCC_PARAMS(o, OccMotionCtrl)->skip != 0) {
        return;
    }
    switch (s->target) {
    case 0:
        h = FldObjGet(s->id);
        if (h == NULL) {
            OccEnd(o);
            return;
        }
        break;
    case 1:
        e = ChrTenantGet(s->id);
        if (e == NULL || e->model == NULL) {
            OccEnd(o);
            return;
        }
        h = e->model;
        break;
    case 2:
    case 3:
        if (s->target == 2) {
            e = ChrGetById(s->id);
        } else {
            e = ChrGetByPartySlot(s->id);
        }
        if (e == NULL) {
            OccEnd(o);
            return;
        }
        h = e->model;
        break;
    default:
        OccEnd(o);
        return;
    }
    switch (s->op) {
    case 1:
        ChrMotFlagsSet(h, 8, 1);
        break;
    case 2:
        ChrMotFlagsSet(h, 8, 0);
        break;
    case 5:
        ChrMotApplyRootMove(e);
    case 0:
        ChrMotFlagsSet(h, 4, 0);
    case 3:
        if (e != NULL) {
            e->flags &= ~0x40;
        }
        ChrMotFlagsSet(h, 3, 1);
        ChrMotFlagsSet(h, 0xC, 0);
        if (s->value < 0.1f) {
            ChrMotStateReset(h);
        }
        break;
    case 4:
        if (e != NULL) {
            e->motSpeed = s->value;
        }
        break;
    }
}
