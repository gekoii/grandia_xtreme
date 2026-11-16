#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "gph.h"


void FxAfterimageSnapshot(BtlUnit *e, f32 r, f32 g, f32 b) {
    FxAfterimageSnap *q = &fxAfterimageSnap;
    GphModel *s;

    if (!(e->params->bitCsf & 0x800)) {
        if (e->body >= 0) {
            e = &btlUnits[e->body];
        }
        if (e->motion != 0) {
            BtlChrModelUpdate(e);
            s = e->model;
            q->pos[0] = s->pos[0];
            q->rot[0] = s->rot[0];
            q->pos[1] = s->pos[1];
            q->rot[1] = s->rot[1];
            q->pos[2] = s->pos[2];
            q->motion = s->motion[0].motion;
            q->scale[0] = s->scale[0];
            q->rot[2] = s->rot[2];
            q->unit = e;
            q->scale[1] = s->scale[1];
            q->scale[2] = s->scale[2];
            q->frame = s->motion[0].frame;
            q->color[0] = 2.0f * r;
            q->color[1] = 2.0f * g;
            q->color[2] = 2.0f * b;
            q->color[3] = 128.0f;
        }
    }
}
