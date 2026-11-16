/* cc: ee-gcc2.9-991111-01 */
/* rodata: 0x00671188-0x00671190 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccMotionTimedForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccMotionTimed *a = OCC_PARAMS(obj, OccMotionTimed);
    ChrObj *o = NULL;
    GphModel *m;
    s16 t;

    switch (a->target) {
    case 0:
        m = FldObjGet(a->id);
        if (m == 0) {
            OccEnd(obj);
            return;
        }
        break;
    case 1:
        o = ChrTenantGet(a->id);
        if (o == NULL) {
            OccEnd(obj);
            return;
        }
        m = (s32)o->model;
        o->flags |= 0x40;
        break;
    case 2:
    case 3:
        if (a->target == 2) {
            o = ChrGetById((u16)a->id);
        } else {
            o = ChrGetByPartySlot(a->id);
        }
        if (o == NULL) {
            OccEnd(obj);
            return;
        }
        m = (s32)o->model;
        o->flags |= 0x40;
        break;
    default:
        OccEnd(obj);
        return;
    }
    t = a->mot;
    if (a->blend != 0) {
        ChrMotSet(o, (u8)t);
    } else {
        ChrMotSetImmediate(o, (u8)t);
    }
    ChrMotFlagsSet(o->model, 1, 0);
    ChrMotFlagsSet(o->model, 2, 1);
    a->length = ChrMotGetLastFrame(m);
    a->frame.i = 0;
    ChrMotSetFrame(m, a->frame.f);
    a->model = m;
    a->turnStep = a->turn / a->length;
    if (obj->flags & 0x200) {
        if (o != NULL) {
            if (a->speed >= 0.099) {
                o->motSpeed = a->speed;
            }
            a->turnStep *= o->motSpeed;
        }
    }
    if (o != NULL) {
        a->turnStep *= o->motSpeed;
    }
    obj->timer = 1;
}
