#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccChrFlagForward(OccEntry *o) {
    OccChrFlag *s = OCC_PARAMS(o, OccChrFlag);
    ChrObj *e;
    ChrObj *f;

    switch (OCC_PARAMS(o, OccChrFlag)->target) {
        case 0:
        e = ChrTenantGet(s->id);
        if (e == NULL) {
            return;
        }
        switch (s->op) {
        case 1:
        ChrTenantAiSetMode(e, s->aiMode);
        break;
        case 2:
            if (s->clear == 0) {
                e->flags |= 0x20;
            } else {
                e->flags &= ~0x20;
            }
            break;
        case 3:
            if (s->clear == 0) {
                e->flags |= 0x8;
            } else {
                e->flags &= ~0x8;
            }
            break;
        case 4:
            if (s->clear == 0) {
                e->flags |= 0x4;
            } else {
                e->flags &= ~0x4;
            }
            break;
        case 5:
            if (s->clear == 0) {
                e->flags |= 0x400;
            } else {
                e->flags &= ~0x400;
            }
            break;
        case 6:
            if (s->clear == 0) {
                e->flags |= 0x800;
            } else {
                e->flags &= ~0x800;
            }
            break;
        case 7:
            if (s->clear == 0) {
                e->flags |= 0x200;
            } else {
                e->flags &= ~0x200;
            }
            break;
        case 8:
            if (s->clear == 0) {
                e->flags &= ~0x1002;
            } else {
                e->flags |= 0x1002;
            }
            break;
        case 10:
            if (s->clear == 0) {
                e->actFlags |= 1;
            } else {
                e->vel[2] = 0;
                e->vel[0] = 0;
                e->vel[1] = 0;
                e->actFlags &= ~1;
            }
            break;
        case 11:
            if (s->clear == 0) {
                e->actFlags &= ~2;
            } else {
                e->actFlags |= 2;
            }
            break;
        case 13:
            if (s->clear == 0) {
                e->flags |= 0x80;
            } else {
                e->flags &= ~0x80;
        }
        break;
            }
            break;
        case 1:
        f = ChrGetById(s->id);
        if (f == NULL) {
            return;
        }
        switch (s->op) {
        case 0:
            break;
        case 3:
            if (s->clear == 0) {
                f->flags |= 0x8;
            } else {
                f->flags &= ~0x8;
            }
            break;
        case 4:
            if (s->clear == 0) {
                f->flags |= 0x4;
            } else {
                f->flags &= ~0x4;
            }
            break;
        case 5:
            if (s->clear == 0) {
                f->flags |= 0x400;
            } else {
                f->flags &= ~0x400;
            }
            break;
        case 6:
            if (s->clear == 0) {
                f->flags |= 0x800;
            } else {
                f->flags &= ~0x800;
            }
            break;
        case 13:
            if (s->clear == 0) {
                f->flags |= 0x80;
            } else {
                f->flags &= ~0x80;
        }
        break;
        }
        break;
    }
}
