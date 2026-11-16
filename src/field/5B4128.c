#include "common.h"
#include "protos/field.h"
#include "field/event.h"

extern s16 occKindPassTable[];


void OccUpdate(void) {
    s32 pass;
    s32 i;
    OccEntry *e;
    s32 r;
    s32 r2;

    if (EvtSystemFlagGet(0x6F) == 1) {
        return;
    }
    for (pass = 0; pass < 2; pass++) {
        for (i = 0; i < occTable->count; i++) {
            e = &occTable->items[i];
            if (occKindPassTable[e->kind] != pass) {
                continue;
            }
            if (e->flags & OCC_ACTIVE) {
                switch (e->state) {
                case OCC_SLEEP:
                    break;
                case OCC_WAKE:
                    if (OccCondCheck(&e->cond) == 0) {
                        e->state = OCC_SLEEP;
                        e->flags &= ~(OCC_ACTIVE | OCC_STOP_LOOP);
                        break;
                    }
                    e->state = OCC_FIRST_WAIT;
                    e->flags &= ~OCC_STOP_LOOP;
                    e->timer = e->unk1A;
                case OCC_FIRST_WAIT:
                    if (e->timer > 0) {
                        e->timer--;
                        break;
                    }
                    e->state = OCC_BEGIN_WAIT;
                    e->timer = e->unk1C;
                    OccCallBegin(e);
                    if (!(e->flags & OCC_ACTIVE)) {
                        break;
                    }
                case OCC_BEGIN_WAIT:
                /* state3/state5: the loop and backward paths jump into these cases */
                state3:
                    if (e->timer > 0) {
                        e->timer--;
                        break;
                    }
                    e->state = OCC_FORWARD;
                    OccCallForwardStart(e);
                    if (e->flags & OCC_ACTIVE) {
                        if (e->timer > 0) {
                            e->timer--;
                            OccCallForward(e);
                        } else {
                            OccEnd(e);
                        }
                    }
                    break;
                case OCC_FORWARD:
                    if (e->timer > 0) {
                        e->timer--;
                        OccCallForward(e);
                        break;
                    }
                    if (e->flags & 0x40) {
                        r2 = EvtAreaGetResult(e->unk6);
                        if (((r2 & 0xF) >> 1) == 3 && (r2 & 1)) {
                            break;
                        }
                        e->state = OCC_BACKWARD_WAIT;
                        e->timer = e->unk1E;
                        goto state5;
                    }
                    if ((e->flags & OCC_LOOP) && !(e->flags & OCC_STOP_LOOP)) {
                        e->state = OCC_BEGIN_WAIT;
                        e->timer = e->unk1C;
                        goto state3;
                    }
                    OccEnd(e);
                    break;
                case OCC_BACKWARD_WAIT:
                state5:
                    if (e->timer > 0) {
                        e->timer--;
                        break;
                    }
                    e->state = OCC_BACKWARD;
                    OccCallBackwardStart(e);
                case OCC_BACKWARD:
                    if (e->timer > 0) {
                        e->timer--;
                        OccCallBackward(e);
                        break;
                    }
                    if ((e->flags & OCC_LOOP) && !(e->flags & OCC_STOP_LOOP)) {
                        e->state = OCC_BEGIN_WAIT;
                        e->timer = e->unk1C;
                        goto state3;
                    }
                    OccEnd(e);
                    break;
                default:
                    e->state = OCC_SLEEP;
                    e->flags &= ~(OCC_ACTIVE | OCC_STOP_LOOP);
                    break;
                }
            } else {
                /* flags & 0x82 == 2 through the doubleword (ld): a u16 test is 88 words off */
                if ((OCC_TERM1_FLAGS(e) & 0x82000000000000) == 0x2000000000000
                    && OccCondCheck(&e->cond) == 1) {
                    e->flags |= 0x80;
                    OccWake(i);
                }
                r = EvtAreaGetResult(e->unk6);
                if (e->kind == 12) {
                    if (r & 0xF0) {
                        e->params.trap.hit = r >> 4;
                        OccWake(i);
                    }
                } else {
                    switch ((r & 0xE) >> 1) {
                    case 0:
                    case 1:
                    case 3:
                    case 4:
                        if (r & 1) {
                            OccWake(i);
                        }
                        break;
                    case 2:
                        if (r & 1) {
                            OccWake(i);
                        }
                        break;
                    }
                }
            }
        }
    }
}
