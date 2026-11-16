#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/event.h"


void ChrExtraPartsUpdate(ChrObj *c) {
    s32 ok;
    GphPart *p;

    ok = c->ctrlMode != 2;
    if (EvtScnrIsRunning() == 1) {
        if ((evtScnrCur->id & 0xF000) != 0xE000) {
            ok = 0;
        }
        if (EvtSystemFlagGet(0x84) == 1) {
            ok = 0;
        }
    }
    if (camCtl.aiType == CAM_AI_TOWN) {
        ok = 0;
    }
    if (camCtl.unk5 < 2) {
        ok = 0;
    }
    if (ok == 1) {
        p = ChrModelFindPart(c->model, 0x366);
        if (p != NULL) {
            if (c->actFlags & 0x20000) {
                p->flags |= 8;
            } else if (p->flags & 8) {
                p->flags ^= 8;
            }
        }
    } else {
        p = ChrModelFindPart(c->model, 0x366);
        if (p != NULL) {
            if (c->actFlags & 0x8000) {
                if (p->flags & 8) {
                    p->flags ^= 8;
                }
            } else {
                p->flags |= 8;
            }
        }
        p = ChrModelFindPart(c->model, 0x302);
        if (p != NULL) {
            if (c->actFlags & 0x4000) {
                if (p->flags & 8) {
                    p->flags ^= 8;
                }
            } else {
                p->flags |= 8;
            }
        }
    }
}
