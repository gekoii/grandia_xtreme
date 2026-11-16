#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/event.h"
#include "field/state.h"

typedef struct {
    ChrObj *c;
    s32 x4;
    s32 x8;
} Grab;

extern Grab chrTalkWork;


void ChrTalkUpdate(Pad *in) {
    FVECTOR v;
    ChrObj *c;
    ChrObj *pl;
    Grab *g;

    if (EvtScnrIsRunning() == 0 && (in->repeat & 0x20)) {
        c = ChrTenantFindTalkTarget(v);
        if (c == NULL) {
            return;
        }
        pl = ChrGetByNo(1);
        if (pl != NULL) {
            __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(pl->vel) : "memory");
            pl->speed = 0.0f;
            pl->mot = 1;
        }
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(c->vel) : "memory");
        c->speed = 0.0f;
        if (EvtSystemFlagGet(c->unk19 + 0xA0) == 1) {
            EvtScnrCall((u16)c->id);
            return;
        }
        if (c->actFlags & 2) {
            c->mot = 5;
        } else {
            c->mot = 4;
        }
        g = &chrTalkWork;
        c->flags |= 0x140;
        chrTalkWork.c = c;
        g->x8 = c->heading;
        if (c->flags & 0x200) {
            ChrTurnToPlayer(c);
        }
        EvtScnrCall((u16)c->id);
        g->x4 = 1;
        return;
    }
    if (EvtScnrIsRunning() == 0) {
        ChrObj *o = chrTalkWork.c;
        if (o == NULL) {
            return;
        }
        o->flags = (o->flags & 0xFEBF) | 0x80;
        o->moveDir = chrTalkWork.x8;
        if (o->actFlags & 2) {
            o->mot = 3;
        } else {
            o->mot = 0;
        }
        chrTalkWork.c = NULL;
        return;
    }
    g = &chrTalkWork;
    if (chrTalkWork.c == NULL) {
        return;
    }
    if (EvtSystemFlagGet(0x84) != 0) {
        return;
    }
    if (evtScnrCur->flags & 2) {
        return;
    }
    if (g->x4 == 0) {
        return;
    }
    g->x4 = ChrTalkApproach(chrTalkWork.c);
}
