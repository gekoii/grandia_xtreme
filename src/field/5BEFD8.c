#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccChrFlagsForward(u8 *arg0) {
    OccChrFlags *a = OCC_PARAMS(arg0, OccChrFlags);
    ChrObj *o;
    ChrObj *p;

    switch (a->target) {
    case 0:
        o = ChrTenantGet(a->id);
        if (o == NULL) {
            break;
        }
        if (a->op[0] == 1) {
            o->flags |= 0x8;
        } else if (a->op[0] == 2) {
            o->flags &= ~0x8;
        }
        if (a->op[1] == 1) {
            o->flags |= 0x4;
        } else if (a->op[1] == 2) {
            o->flags &= ~0x4;
        }
        if (a->op[2] == 1) {
            o->flags |= 0x400;
        } else if (a->op[2] == 2) {
            o->flags &= ~0x400;
        }
        if (a->op[3] == 1) {
            o->flags |= 0x800;
        } else if (a->op[3] == 2) {
            o->flags &= ~0x800;
        }
        if (a->op[5] == 1) {
            o->flags |= 0x80;
        } else if (a->op[5] == 2) {
            o->flags &= ~0x80;
        }
        if (a->op[6] == 1) {
            o->flags |= 0x20;
        } else if (a->op[6] == 2) {
            o->flags &= ~0x20;
        }
        if (a->op[7] == 1) {
            o->flags &= ~0x1002;
        } else if (a->op[7] == 2) {
            o->flags |= 0x1002;
        }
        if (a->op[8] == 1) {
            o->actFlags |= 1;
        } else if (a->op[8] == 2) {
            o->actFlags &= ~1;
            o->vel[0] = 0;
            o->vel[1] = 0;
            o->vel[2] = 0;
        }
        if (a->op[9] == 1) {
            o->flags |= 0x200;
        } else if (a->op[9] == 2) {
            o->flags &= ~0x200;
        }
        if (a->op[10] == 1) {
            o->actFlags &= ~2;
        } else if (a->op[10] == 2) {
            o->actFlags |= 2;
        }
        break;
    case 1:
    case 2:
        if (a->target == 1) {
            p = ChrGetById((u16)(a->id + 1));
        } else if (a->target == 2) {
            p = ChrGetByPartySlot(a->id);
        } else {
            break;
        }
        if (p == NULL) {
            break;
        }
        if (a->op[0] == 1) {
            p->flags |= 0x8;
        } else if (a->op[0] == 2) {
            p->flags &= ~0x8;
        }
        if (a->op[1] == 1) {
            p->flags |= 0x4;
        } else if (a->op[1] == 2) {
            p->flags &= ~0x4;
        }
        if (a->op[2] == 1) {
            p->flags |= 0x400;
        } else if (a->op[2] == 2) {
            p->flags &= ~0x400;
        }
        if (a->op[3] == 1) {
            p->flags |= 0x800;
        } else if (a->op[3] == 2) {
            p->flags &= ~0x800;
        }
        if (a->op[5] == 1) {
            p->flags |= 0x80;
        } else if (a->op[5] == 2) {
            p->flags &= ~0x80;
        }
        break;
    }
}
