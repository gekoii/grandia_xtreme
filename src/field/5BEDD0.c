/* rodata: 0x00671190-0x00671198 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"

void ChrMotApplyRootMove(ChrObj *);

void OccMotionTimedForward(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccMotionTimed *a = OCC_PARAMS(obj, OccMotionTimed);
    ChrObj *o = NULL;
    s32 m;
    s16 t;

    m = a->model;
    switch (a->target) {
    case 1:
        o = ChrTenantGet(a->id);
        break;
    case 2:
    case 3:
        if (a->target == 2) {
            o = ChrGetById((u16)a->id);
        } else {
            o = ChrGetByPartySlot(a->id);
        }
        break;
    }
    if (o != NULL) {
        a->frame.f += o->motSpeed;
    } else {
        a->frame.f += 1.0f;
    }
    if (a->frame.f > a->length) {
        if (a->turnEnd != 0 && o != NULL) {
            o->heading = o->heading + a->turnEnd * 182.04445f;
        }
        o->motSpeed = 1.0f;
        if (obj->flags & 0x200) {
            if (a->unk14 >= 0.099 && o != NULL) {
                o->motSpeed = a->speed;
            }
        }
        if (a->rootMove != 0) {
            ChrMotApplyRootMove(o);
        }
        t = a->nextMot;
        if (t < 0) {
            t = 0;
            if (o != NULL) {
                o->flags &= ~0x40;
            }
        }
        if (a->blend != 0) {
            ChrMotSet(o, (u8)t);
        } else {
            ChrMotSetImmediate(o, (u8)t);
        }
        ChrMotSetFrame(m, 0.0f);
        obj->timer = 0;
    } else {
        ChrMotSetFrame(m, a->frame.f);
        if (a->turn != 0 && o != NULL) {
            o->heading = o->heading + a->turnStep * 182.04445f;
        }
        obj->timer = 1;
    }
}
