#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

void ChrTenantAiSetMode(ChrObj *a, u16 mode) {
    ChrEnemyWork *s;
    GphModel *o;

    if (mode == 2 && a->aiMode == mode) {
        return;
    }
    if (a->opState == 4) {
        return;
    }
    if (a->opState == 5) {
        return;
    }
    s = a->sense;
    if (s != NULL) {
        if (mode == 1) {
            s->flags |= 4;
        } else if (s->flags & 4) {
            s->flags ^= 4;
        }
    }
    o = a->model;
    a->opPc = 0;
    a->opState = 0;
    a->opTimer = 0;
    o->scale[0] = 1.0f;
    o->scale[1] = 1.0f;
    o->scale[2] = 1.0f;
    a->aiMode = mode;
    if (mode == 2 && a->sense != NULL) {
        ChrTenantPlaySe(CHR_SENSE(a)->charDef->alertSe, (s32)o->pos);
    }
}
