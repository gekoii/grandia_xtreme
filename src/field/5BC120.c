#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccShakeForwardStart(OccEntry *o) {
    OccShake *s = OCC_PARAMS(o, OccShake);
    GphModel *e;
    s32 i;
    s32 m;
    s32 t;

    e = FldObjGet(OCC_PARAMS(o, OccShake)->id);
    if (e == NULL) {
        OccEnd(o);
        return;
    }
    if (s->part != 0) {
        GphPart *n = ChrModelFindPart(e, s->part);
        s->base[0] = n->rot[0];
        s->base[1] = n->rot[1];
        s->base[2] = n->rot[2];
        s->rot[0] = n->rot[0];
        s->rot[1] = n->rot[1];
        s->rot[2] = n->rot[2];
    } else {
        s->base[0] = e->rot[0];
        s->base[1] = e->rot[1];
        s->base[2] = e->rot[2];
        s->rot[0] = e->rot[0];
        s->rot[1] = e->rot[1];
        s->rot[2] = e->rot[2];
    }
    o->timer = 1;
    m = 0;
    for (i = 0; i < 3; i++) {
        t = s->amp[i] < 0 ? -s->amp[i] : s->amp[i];
        if (m < t) {
            m = t;
        }
    }
    switch (s->decayMode) {
    case 0:
        s->decay = 0;
        break;
    case 1:
        s->decay = m * 0.1f;
        break;
    case 2:
        s->decay = m * 0.05f;
        break;
    case 3:
        s->decay = m * 0.2f;
        break;
    }
    for (i = 0; i < 3; i++) {
        if (s->amp[i] > 0) {
            s->dir[i] = 1;
            s->swing[i] = s->amp[i];
            s->speed[i] = s->swing[i];
        } else if (s->amp[i] < 0) {
            s->dir[i] = -1;
            t = s->amp[i];
            s->swing[i] = -t;
            s->speed[i] = t;
        } else {
            s->dir[i] = 0;
        }
        s->wait[i] = s->delay[i];
    }
}
